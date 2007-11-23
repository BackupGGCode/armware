// ARMware - an ARM emulator
// Copyright (C) <2007>  Wei Hu <wei.hu.tw@gmail.com>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

namespace ARMware
{
#if ENABLE_DYNAMIC_TRANSLATOR
  ExecResultEnum
  exec_CRT_LOAD_TC(Inst const inst)
  {
    return exec_CRT_TC<LOAD>(inst);
  }
  
  ExecResultEnum
  exec_CRT_STORE_TC(Inst const inst)
  {
    return exec_CRT_TC<STORE>(inst);
  }
  
  template<IsLoad T_is_load>
  void
  exec_CRT_DT(Inst const inst, bool const is_last)
  {
    DTFunc_t dt_func;
    
    switch (T_is_load)
    {
    case LOAD:
      dt_func = reinterpret_cast<DTFunc_t>(exec_CRT_LOAD_TC);
      break;
      
    case STORE:
      dt_func = reinterpret_cast<DTFunc_t>(exec_CRT_STORE_TC);
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
    
    Label * const routine_label =
      gp_chunk->add_new_label(new Label(static_cast<LabelRoutine *>(0), dt_func));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRCall_1 *>(0),
                                                 routine_label,
                                                 gp_chunk->find_const_var(inst.mRawData)));
    
    Variable * const tmp1 = gp_chunk->get_new_temp();
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRRetrieve *>(0), 1, tmp1));
    
    Label * const table_base_label =
      gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0)));
    
    MIR * const jmp_mir = gp_chunk->insert_switch_statement_DT(tmp1, table_base_label);
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // Create jump table.
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_NORMAL:
    Label * const label_ER_NORMAL =
      gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0)));
    
    MIR * const table_base_mir =
      gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                                   label_ER_NORMAL));
    
    table_base_label->set_mir_inst(table_base_mir);
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_EXCEPTION:
    Label * const label_ER_EXCEPTION =
      gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0)));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                                 label_ER_EXCEPTION));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_NORMAL_CONT_EXEC_IN_CHUNK:
    //
    // This value is not possible for CRT, thus we set the value of this label to 0.
    Label * const label_ER_NORMAL_CONT_EXEC_IN_CHUNK =
      gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0), 0));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                                 label_ER_NORMAL_CONT_EXEC_IN_CHUNK));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_NORMAL_MODIFY_PC:
    //
    // This value is not possible for CRT, thus we set the value of this label to 0.
    Label * const label_ER_NORMAL_MODIFY_PC =
      gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0), 0));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                                 label_ER_NORMAL_MODIFY_PC));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_IO_IRQ_OCCUR:
    //
    // This value is not possible for CRT, thus we set the value of this label to 0.
    Label * const label_ER_IO_IRQ_OCCUR =
      gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0), 0));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                                 label_ER_IO_IRQ_OCCUR));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_IO_FIQ_OCCUR:
    //
    // This value is not possible for CRT, thus we set the value of this label to 0.
    Label * const label_ER_IO_FIQ_OCCUR =
      gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0), 0));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                                 label_ER_IO_FIQ_OCCUR));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_CHUNK_DISAPPEAR:
    //
    // This value is not possible for CRT, thus we set the value of this label to 0.
    Label * const label_ER_CHUNK_DISAPPEAR =
      gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0), 0));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                                 label_ER_CHUNK_DISAPPEAR));
    
    // :NOTE: Wei 2004-Nov-5:
    //
    // End of the basic block for the jump table.
    gp_chunk->next_mir_is_leader();
    
    jmp_mir->add_jump_target_label(label_ER_NORMAL);
    jmp_mir->add_jump_target_label(label_ER_EXCEPTION);
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // Jump target implementation.
    //
    // :NOTE: Wei 2004-Oct-03:
    //
    // The possible value returned from 'CRT' are:
    //
    // (1) ER_EXCEPTION
    // (2) ER_NORMAL
    //
    // if (1), then ARMware will return to the interpreter,
    // otherwise, (2) will continue execuating in this chunk.
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // ER_EXCEPTION
    
    label_ER_EXCEPTION->set_mir_inst(gp_chunk->append_mir<MIR_FOR_NORMAL>(
                                       new MIR(static_cast<MIRReturn *>(0),
                                               gp_chunk->find_const_var(ER_EXCEPTION))));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // ER_NORMAL.
    
    if (true == is_last)
    {
      label_ER_NORMAL->set_mir_inst(
        gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRReturn *>(0),
                                                     gp_chunk->find_const_var(ER_NORMAL))));
    }
    else
    {
      label_ER_NORMAL->reset_to_arm_first_inst(gp_chunk->next_arm_inst_info());
      
      gp_chunk->add_pending_back_patch_label(label_ER_NORMAL);
    }
  }
#endif
}

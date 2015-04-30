# 特色 #

  * Serial Console 部份:
    * 可以從 serial console 輸入.
  * 模擬環境部份:
    * 完成 ARM Architecture version 4 Instruction Set 的模擬.
    * 具備 dynamic compiler 技術.
    * 具備 threaded code 最佳化技術.

# 簡介 #

ARMware 是個類似 VMware 或 Bochs 的硬體平台模擬器, 不過相對於 VMware 或 Bochs 所模擬的 x86 平台, ARMware 提供了一個模擬 ARM 處理器平台的環境. 目前 ARMware 所能模擬的核心為 Intel StrongARM SA-1110 (模擬 Intel Xscale 的功能正在開發中). 並且 ARMware 以 Compaq iPaq H3600
為對象, 提供了一個模擬 H3600 的執行環境 (未來的 Intel Xscale 模擬環境將會以 HP iPaq H5500 系列為模擬對象).

目前 ARMware 所模擬的元件包括:

  * ARM architecture version 4 core
    * 支援所有的 ARM instruction set (但不支援 Thumb instruction set)
    * 支援所有的 processor mode:
      * User mode
      * Fast Interrupt mode
      * Interrupt mode
      * Supervisor mode
      * Abort mode
      * Undefined Instruction mode
      * System mode
  * ARM standard system coprocessor (CP15)
  * Memory Management Unit (MMU)
    * 支援所有的 address translation:
      * Section translation
      * Large page translation
      * Small page translation
      * Tiny page translation
    * 支援所有的 MMU fault:
      * Translation fault
      * Domain fault
      * Access permission fault
      * Alignemnt fault
  * Translation Lookaside Buffer (TLB)
    * 為了增加效能, 所以我採用 hash table 搭配 full associated 的方式來實作 SA-1110 內部的 TLB.
  * LCD Controller
  * DMA Controller
  * EGPIO
  * Operation System Timer
  * Reset Controller
  * Real Time Clock (RTC)
  * GPIO Controll Block
  * Interrupt Controller
  * Power Management
    * 支援 idle mode
  * Serial Port 1
    * 與 Atmel micro-controller 相連
  * Serial Port 3
    * 與 Serial console 相連
  * Intel StrataFlash J3 Flash ROM
    * 完整的 Common Flash Interface (CFI) 支援
    * 完整的 Write State Machine (WSM) 支援, 包括
      * read array
      * read ID codes
      * block lock
      * block unlock
      * read status register
      * read block register
      * byte/word program
      * write to buffer
      * block erase
        * 使用 GNU dbm 作為 ROM file database (螢幕快照)
        * 使用 XML 作為 ROM file database (螢幕快照)
  * Atmel micro-controller
    * 部份功能支援, 包括
      * 取得 version 資訊
      * 設定或取得 LCD back light
  * LCD screen
    * 使用 Qt (螢幕快照)
    * 使用 Gtk (螢幕快照)
  * Touch panel

# 優點 #

  * 內建一個 threaded code engine, 會把執行過的 ARM machine code 給 cache 起來, 因此可以加快執行的速度.
  * 並且更內建一個 dynamic compiler (動態編譯器), 這個 compiler 會動態的把 ARM 的 machine code 編譯成 x86 的 machine code, 因此可以加快執行的速度. 目前實作在這個 dynamic compiler 的最佳化技術總計有下列數項:
    * Redundant condition code calculation elimination
    * Global grouping conditional execuating instruction
    * Redundant jump elimination
    * Dead code elimination
    * Constant folding
    * Global Common Subexpression Elimination
    * Global redundany memory operation elimination
    * Algebraic canonicalization
    * Global SSA form based linear scan register allocation
  * 除此之外, 我也以 x86 assembly codes 重寫了 ARMware 的某些部份, 希望能夠更加快執行的速度:
    1. 我完全以 Intel 的 MMX 及 SSE2 指令集來完成在 HP iPaq 上的 LCD 旋轉 270 度的動作 (這也就是說, 你必須在一台具備 MMX 及 SSE2 指令集的 x86 CPU 上來執行 ARMware, 例如 Pentium M 以及 Pentium 4).
    1. 我也以 x86 assembly codes 來最佳化 ARM 平台的 condition codes 計算.
  * 內建一個記憶體管理子系統, 並且經過冗常的測試, ARMware 縱使在加入 dynamic compiler 功能的情況下也幾乎沒有 memory leak 的情形.
  * 具有 Qt (螢幕快照) 或 Gtk (螢幕快照) 的使用者介面, 但由於 Qt 的 license 問題, 所以目前我只釋放出以 Gtk 為使用者介面的 ARMware.
  * 具備一個 serial console 視窗 (螢幕動畫), 因此可以從該視窗中看到 ARMware 的 serial console 輸出, 同時也可以從該視窗中輸入使用者命令.
  * 可以使用 GNU dbm (螢幕快照) 或 xml (螢幕快照) 為機器資料的資料庫. 但同樣的, 由於 GNU dbm 的 license 問題, 所以我目前只釋放出以 xml 為機器資料庫的 ARMware.
  * 符合 i18n 標準的繁體中文化介面, 目前的繁體中文翻譯有 Big5 以及 UTF-8.
  * 具備一個完整的模擬燒錄 Flash ROM 工具(螢幕快照), 可用來模擬真實世界中的燒錄 Flash 動作, 並且使用燒錄出來的 Flash ROM 檔案來啟動 ARMware.
  * 每個使用者的機器資料庫分別存放在使用者個家目錄中, 因此不同使用者可以擁有各自的機器資料庫而不會衝突.

# ARMware 執行流程 #

  1. 當第一次碰到從未執行過的 ARM code 時, ARMware 會先使用內建的 interpreter 來解釋這些 ARM codes.
  1. 之後經由 ARMware 內建的 profiler 來決定那些 EBB (extended basic block) 值得被 threaded-code 化, 並且將該 EBB 餵給 threaded code engine 來產生相對於該 EBB 的 threaded codes.
  1. 在產生 threaded codes 之後, 當 ARMware 再度碰到這塊 EBB 時, 就會使用新產生的 threaded codes 去解釋這個 EBB. 如果在執行 threaded codes 的時候, 跳到一個還沒被 threaded code 化的 EBB 時, ARMware 就回到 interpreter 模式去解釋.
  1. 再經由 ARMware 內建的 profiler 來決定那些 EBB 值得被 dynamic-compiled 化, 並且將該 EBB 餵給 ARMware 內建的 dynamic compiler engine 來產生相對於該 EBB 的 x86 machine codes.
  1. 在產生 x96 machine codes 之後, 當 ARMware 再度碰到這塊 EBB 時, 就會使用新產生的 x86 machine codes 來解釋這個 EBB. 如果在執行這段 x86 machine codes 的過程中, 碰到一個還沒被 dynamic compiled 的 EBB, 就會回到原始的 interpreter 模式或是先前的 threaded code 模式去解釋.

# 缺點 #

  1. 沒有網路功能.
  1. 沒有聲音.

# 架構圖 #

![http://lh4.google.com/wei.hu.tw/RzxreLZHyQI/AAAAAAAAAG4/WmNpG5AS4RM/arch.png](http://lh4.google.com/wei.hu.tw/RzxreLZHyQI/AAAAAAAAAG4/WmNpG5AS4RM/arch.png)

# 套件內容 #

ARMware 除了包含一個主程式執行檔外, 還有執行所需的函式庫, 以及數張 ARMware 所需用到的圖檔, 以及兩個繁體中文的 locale 檔.

# 系統需求 #

  * 硬體: Pentium M 或 Pentium 4 (因為 ARMware 需要使用 MMX 及 SSE2 指令集)
  * 函式庫: 由於 ARMware 是以組合語言及純 C++ 語言寫成, 並且以 gcc-4.0 來進行編譯, 因此如果你想要嘗試 ARMware 的話, 你所需要的函式庫需求為:

  * libc6 (>= 2.3.5)
  * libgcc1 (>= 4.0.0)
  * gtkmm (>= 2.4)
  * glibmm (>= 2.4)
  * libsigc++ (>= 2.0)
  * libxml++ (>= 2.6)

在這邊需要注意一點的是, 由於 gcc-3.3 及 gcc-4.0 在 C++ ABI 上的不相容, 所以上述的 4 個 C++ 函式庫 (gtkmm, glibmm, libsigc++, libxml++) 都必須是以 gcc-4.0 以上版本來編譯的才行. (gcc-3.4 的 C++ ABI 跟 gcc-4.0 的一樣, 所以你也可以嘗試用 gcc-3.4 來編譯這 4 個 C++ 函式庫)

就我所知, 目前的 debian stable, testing, unstable 以及 ubuntu hoary 都還是以 gcc-3.3 為預設的編譯環境, 而 ubuntu breezy 則已經切換到 gcc-4.0 了. 所以你可以抓取 breezy 的套件來直接安裝, 或者是自行取得這 4 個函式庫的原始碼來自行以 gcc-4.0 來進行編譯.

# Linux Kernel Patch #

由於 iPaq 的 touch panel 品質不是很好, 因此 Linux kernel 會捨棄掉第一次按下 touch panel 所獲得的 (x, y) 值. 但是 ARMware 是個軟體模擬器, 因此不需要 Linux kernel 的這個動作. 所以我修改了 Linux kernel 的一些 codes, 包括上述的 touch panel 問題以及讓 kernel message 傾印到 serial console 中.

如果你想要執行 ARMware 並且自行編譯 Linux kernel 的話, 你就必須要下載這個 [kernel patch](http://armware.googlecode.com/files/kernel-patch.patch).

註: 這個 kernel patch 是 for kernel 的 2.4.19-rmk6-pxa1-hh30 版本.

# 使用方法 #

以下所有的檔案都可以從下面的 下載區 中下載.

  1. 安裝好 ARMware.
  1. 下載 [bootldr.bin](http://armware.googlecode.com/files/bootldr.bin) 以及 [root.bin](http://armware.googlecode.com/files/root.bin), 並且放到某一個目錄下.
  1. 執行 armware
  1. 在右邊的 "機器名稱" 視窗中, 按下滑鼠右鍵, 選擇 "新增".
  1. 在 "基本資訊" 中, 填入你想要的機器名稱.
  1. 在 "Flash ROM" 中, 按下 "Flash ROM 映像檔" 來選擇最後的 Flash ROM image. 並且在 "Flash ROM 容量" 中鍵入 16 Mbyte. 在下方的 "起始位址" 中鍵入 0, "分割區大小" 鍵入 256 Kbyte, "分割區映像檔" 選擇方才所下載的 bootldr.bin. 最後按下 "新增". 接下來繼續新增 root.bin 檔, 它的參數是: "起始位址": 0x40000, "分割區大小": 16128 Kbyte, "分割區映像檔": root.bin. 完成後, 就可以按下左下方的 "開始燒錄" 來產生你所指定的 Flash ROM image.
  1. 在 "螢幕大小" 中, 填入 寬 240, 高 320. 最後按下 "確定" 來結束機器的設定.
  1. 最後按下啟動, 就可以開始執行 ARM 版本的 linux 了.

# 螢幕快照 #

使用者可以自行調整模擬環境的記憶體大小

![http://lh4.google.com/wei.hu.tw/Rzxr9LZHyVI/AAAAAAAAAHg/o4tccVgd9DQ/dram_size_modify.png](http://lh4.google.com/wei.hu.tw/Rzxr9LZHyVI/AAAAAAAAAHg/o4tccVgd9DQ/dram_size_modify.png)

在調整完記憶體大小之後, ARM linux kernel 確實偵測到較大的記憶體容量

![http://lh4.google.com/wei.hu.tw/Rzxr9LZHyWI/AAAAAAAAAHo/MyfvSSxAtRI/dram_size_prove.png](http://lh4.google.com/wei.hu.tw/Rzxr9LZHyWI/AAAAAAAAAHo/MyfvSSxAtRI/dram_size_prove.png)

使用者可以自行為 serial console 修改喜好的字型, 文字顏色, 以及背景顏色

![http://lh5.google.com/wei.hu.tw/RzxsmbZHyiI/AAAAAAAAAJI/r822uZu8SX8/serial_console_modify.png](http://lh5.google.com/wei.hu.tw/RzxsmbZHyiI/AAAAAAAAAJI/r822uZu8SX8/serial_console_modify.png)

最新的 serial console 可以正確的顯示 ANSI 顏色碼

![http://lh5.google.com/wei.hu.tw/Rzxs7bZHyjI/AAAAAAAAAJQ/Ld5f2DdNctw/serial_console_prove.png](http://lh5.google.com/wei.hu.tw/Rzxs7bZHyjI/AAAAAAAAAJQ/Ld5f2DdNctw/serial_console_prove.png)

ARMware 可以使用 GNU dbm (gdbm) 來當做機器資料庫

![http://lh4.google.com/wei.hu.tw/Rzxr9LZHyXI/AAAAAAAAAHw/f6nxG63su2o/gdbm.png](http://lh4.google.com/wei.hu.tw/Rzxr9LZHyXI/AAAAAAAAAHw/f6nxG63su2o/gdbm.png)

ARMware 可以使用 xml 檔案來當做機器資料庫

![http://lh5.google.com/wei.hu.tw/Rzxs7bZHylI/AAAAAAAAAJg/5lelIYJqdAg/xml.png](http://lh5.google.com/wei.hu.tw/Rzxs7bZHylI/AAAAAAAAAJg/5lelIYJqdAg/xml.png)

使用 Qt 介面

![http://lh5.google.com/wei.hu.tw/Rzxr9bZHyYI/AAAAAAAAAH4/tuNRJac5XKI/qt_running_1.png](http://lh5.google.com/wei.hu.tw/Rzxr9bZHyYI/AAAAAAAAAH4/tuNRJac5XKI/qt_running_1.png)

使用 Gtk 介面

![http://lh5.google.com/wei.hu.tw/Rzxs7bZHykI/AAAAAAAAAJY/9qAz-7Dj6X4/start.png](http://lh5.google.com/wei.hu.tw/Rzxs7bZHykI/AAAAAAAAAJY/9qAz-7Dj6X4/start.png)

關於設計者 (Gtk 介面)

![http://lh5.google.com/wei.hu.tw/RzxrdbZHyPI/AAAAAAAAAGw/Jp9Brx8tXGs/about.png](http://lh5.google.com/wei.hu.tw/RzxrdbZHyPI/AAAAAAAAAGw/Jp9Brx8tXGs/about.png)

選項清單 (Gtk 介面)

![http://lh3.google.com/wei.hu.tw/Rzxr87ZHyUI/AAAAAAAAAHY/SPZbsg8y34E/conf.png](http://lh3.google.com/wei.hu.tw/Rzxr87ZHyUI/AAAAAAAAAHY/SPZbsg8y34E/conf.png)

正在燒錄 Flash ROM (Gtk 介面)

![http://lh5.google.com/wei.hu.tw/RzxrebZHySI/AAAAAAAAAHI/dwxk-8iMo_Q/burning.png](http://lh5.google.com/wei.hu.tw/RzxrebZHySI/AAAAAAAAAHI/dwxk-8iMo_Q/burning.png)

燒錄完成 (Gtk 介面)

![http://lh5.google.com/wei.hu.tw/RzxrebZHyTI/AAAAAAAAAHQ/L1OxBg8tpWA/burning_done.png](http://lh5.google.com/wei.hu.tw/RzxrebZHyTI/AAAAAAAAAHQ/L1OxBg8tpWA/burning_done.png)

一開機執行到 boot loader 的情況 (Gtk 介面)

![http://lh5.google.com/wei.hu.tw/RzxrebZHyRI/AAAAAAAAAHA/RQhd0VVl69w/bootldr.png](http://lh5.google.com/wei.hu.tw/RzxrebZHyRI/AAAAAAAAAHA/RQhd0VVl69w/bootldr.png)

載入 linux kernel, 並且解壓縮完成 (Gtk 介面)

![http://lh6.google.com/wei.hu.tw/RzxsTrZHyZI/AAAAAAAAAIA/xIagMFCoh-4/running_1.png](http://lh6.google.com/wei.hu.tw/RzxsTrZHyZI/AAAAAAAAAIA/xIagMFCoh-4/running_1.png)

kernel 執行完成, 開始執行 /sbin/init (Gtk 介面)

![http://lh3.google.com/wei.hu.tw/RzxsT7ZHyaI/AAAAAAAAAII/lsikEbuDcIA/running_2.png](http://lh3.google.com/wei.hu.tw/RzxsT7ZHyaI/AAAAAAAAAII/lsikEbuDcIA/running_2.png)

進入到 GUI 畫面, 矯正 touch panel (Gtk 介面)

![http://lh3.google.com/wei.hu.tw/RzxsT7ZHybI/AAAAAAAAAIQ/zfqfHiiBCCk/running_3.png](http://lh3.google.com/wei.hu.tw/RzxsT7ZHybI/AAAAAAAAAIQ/zfqfHiiBCCk/running_3.png)

gpe 的初始畫面 1

![http://lh3.google.com/wei.hu.tw/RzxsT7ZHycI/AAAAAAAAAIY/HyVkcmGJCus/running_4.png](http://lh3.google.com/wei.hu.tw/RzxsT7ZHycI/AAAAAAAAAIY/HyVkcmGJCus/running_4.png)

gpe 的初始畫面 2

![http://lh3.google.com/wei.hu.tw/RzxsT7ZHydI/AAAAAAAAAIg/PiAUZY0-jJo/running_5.png](http://lh3.google.com/wei.hu.tw/RzxsT7ZHydI/AAAAAAAAAIg/PiAUZY0-jJo/running_5.png)

gpe 的歡迎畫面

![http://lh4.google.com/wei.hu.tw/RzxsmLZHyeI/AAAAAAAAAIo/mVVochyRWLA/running_6.png](http://lh4.google.com/wei.hu.tw/RzxsmLZHyeI/AAAAAAAAAIo/mVVochyRWLA/running_6.png)

俄羅斯方塊遊戲

![http://lh4.google.com/wei.hu.tw/RzxsmLZHyfI/AAAAAAAAAIw/oDFaoG9Ps_Q/running_7.png](http://lh4.google.com/wei.hu.tw/RzxsmLZHyfI/AAAAAAAAAIw/oDFaoG9Ps_Q/running_7.png)

迷宮遊戲

![http://lh4.google.com/wei.hu.tw/RzxsmLZHygI/AAAAAAAAAI4/5eeVXxohMCY/running_8.png](http://lh4.google.com/wei.hu.tw/RzxsmLZHygI/AAAAAAAAAI4/5eeVXxohMCY/running_8.png)

Terminal

![http://lh4.google.com/wei.hu.tw/RzxsmLZHyhI/AAAAAAAAAJA/XvEIyAeCiKE/running_9.png](http://lh4.google.com/wei.hu.tw/RzxsmLZHyhI/AAAAAAAAAJA/XvEIyAeCiKE/running_9.png)

# 螢幕動畫 #

  1. [開機 (啟動 threaded code & dynamic compiler) (divx4 檔)](http://armware.googlecode.com/files/start.avi)
  1. [從 serial console 輸入命令 (啟動 threaded code & dynamic compiler) (divx4 檔)](http://armware.googlecode.com/files/input.avi)
  1. [矯正 touch panel (沒有啟動 threaded code & dynamic compiler) (divx4 檔)](http://armware.googlecode.com/files/calibrate.avi)
  1. [輸入個人資料 (沒有啟動 threaded code & dynamic compiler) (divx4 檔)](http://armware.googlecode.com/files/personal.avi)
  1. [俄羅斯方塊 (沒有啟動 threaded code & dynamic compiler) (divx4 檔)](http://armware.googlecode.com/files/tetris.avi)
# Cấu trúc project VersatileRemote

Tài liệu này mô tả các thư mục và file chính trong project để bạn dễ hình dung project đang làm gì, ai phụ trách phần nào, và cách các thành phần kết nối với nhau.

## 1. Tổng quan project

Project này là một hệ thống điều khiển từ xa thông minh chạy trên board STM32F429I_DISCO, kết hợp:
- Firmware STM32 viết bằng C/C++
- Giao diện đồ họa TouchGFX
- FreeRTOS để chạy đa task
- Thu/phát tín hiệu hồng ngoại (IR) để học lệnh và phát lại cho TV, điều hòa, thiết bị khác

Nói ngắn gọn, project có 3 lớp chính:
1. Lớp phần cứng/driver: thu nhận và phát tín hiệu IR, khởi tạo LCD, UART, timer, GPIO
2. Lớp điều khiển logic: xử lý tín hiệu IR, quản lý thiết bị và nút bấm
3. Lớp giao diện: màn hình TouchGFX để người dùng bấm nút và học/phát lệnh

---

## 2. Cấu trúc thư mục chính

```text
VersatileRemote/
├── Core/                  # Code firmware chính chạy trên STM32
├── Drivers/               # Driver/BSP/HAL của STM32 và các thành phần phần cứng
├── TouchGFX/              # Giao diện đồ họa bằng TouchGFX
├── Middlewares/           # Thư viện bên thứ ba (FreeRTOS, TouchGFX runtime)
├── EWARM/                 # Project cho IAR Embedded Workbench
├── MDK-ARM/               # Project cho Keil MDK
├── STM32CubeIDE/          # Project cho STM32CubeIDE
├── gcc/                   # Build script và linker cho toolchain GCC
├── readme.md              # Tài liệu tổng quan chức năng
├── STM32F429I_DISCO_REV_D01.ioc  # File cấu hình CubeMX
└── StructureProject.md   # File này
```

---

## 3. Thư mục Core

Thư mục này chứa mã nguồn cốt lõi của firmware chạy trên MCU.

### 3.1 Core/Inc
Thư mục header chứa các định nghĩa API và cấu hình cho ứng dụng.

- Core/Inc/main.h
  - Header chung cho chương trình chính
  - Chứa khai báo các cấu trúc, macro, include liên quan tới STM32 và IR

- Core/Inc/FreeRTOSConfig.h
  - Cấu hình hệ thống RTOS FreeRTOS
  - Quy định stack size, tick, ưu tiên task, các tùy chọn scheduler

- Core/Inc/ir_signal.h
  - Định nghĩa cấu trúc dữ liệu cho tín hiệu IR
  - Chứa enum protocol như NEC, Sony, RC5, RAW
  - Là kiểu dữ liệu trung tâm để lưu tín hiệu thu được hoặc chuẩn bị phát

- Core/Inc/ir_protocols.h
  - Khai báo các hàm giải mã và mã hóa giao thức IR
  - Hỗ trợ NEC, Sony, RC5 và tín hiệu RAW

- Core/Inc/ir_device.h
  - Định nghĩa cấu trúc device và button
  - Mô hình mỗi thiết bị (TV, AC, ...) có nhiều nút bấm và mỗi nút bấm gắn một tín hiệu IR

- Core/Inc/stm32f4xx_hal_conf.h
  - Cấu hình HAL driver cho STM32F4
  - Cho phép bật/tắt module như UART, SPI, I2C, DMA2D, LTDC...

- Core/Inc/stm32f4xx_it.h
  - Khai báo hàm xử lý ngắt cho các interrupt như SysTick, TIM, USART, fault

- Core/Inc/RTE_Components.h
  - File cấu hình cho môi trường phát triển/biên dịch dành cho Keil/MDK

### 3.2 Core/Src
Thư mục source chứa logic thực thi chính.

- Core/Src/main.c
  - File quan trọng nhất của firmware
  - Khởi tạo hệ thống clock, LCD, UART, SPI, I2C, SDRAM, TouchGFX
  - Khởi tạo timer TIM2 để thu tín hiệu IR
  - Tạo và chạy các task FreeRTOS
  - Xử lý callback từ ngắt capture tín hiệu IR
  - Chạy vòng lặp chính để nhận frame IR, giải mã, chia sẻ cho GUI

- Core/Src/ir_signal.c
  - Implement các hàm làm việc với tín hiệu IR
  - Reset signal, append timing, in log debug, phát tín hiệu ra chân IR
  - Đây là nơi thực hiện phát lại tín hiệu hồng ngoại bằng cách điều chế sóng mang 38.4 kHz

- Core/Src/ir_protocols.c
  - Implement decoder/encoder cho các giao thức IR
  - Giải mã tín hiệu thành address/command khi nhận được
  - Mã hóa lại khi cần phát tín hiệu
  - Hỗ trợ NEC, Sony, RC5, RAW

- Core/Src/ir_device.c
  - Quản lý thiết bị và nút bấm
  - Có pool cố định cho device, tránh cấp phát động để tiết kiệm RAM
  - Cho phép thêm nút vào device, tìm nút theo tên, tìm device trong registry

- Core/Src/freertos.c
  - File khung cho FreeRTOS application
  - Trong project này nó chủ yếu là file wrapper/khởi tạo RTOS và không chứa nhiều logic tùy biến

- Core/Src/stm32f4xx_hal_msp.c
  - Cấu hình pin và peripheral cho HAL (USART, GPIO, DMA, LTDC...)

- Core/Src/stm32f4xx_hal_timebase_tim.c
  - Cấu hình thời gian hệ thống dùng timer làm timebase cho HAL

- Core/Src/stm32f4xx_it.c
  - Implement các handler interrupt

- Core/Src/system_stm32f4xx.c
  - Cấu hình hệ thống clock và startup cho STM32F4

- Core/Startup/startup_stm32f429zitx.s
  - File startup assembly cho MCU
  - Chứa vector table và các handler khởi đầu cho boot

---

## 4. Thư mục Drivers

Thư mục này chứa các driver và thư viện phần cứng hỗ trợ board.

### 4.1 Drivers/BSP
- Chứa driver cho board support package của STM32F429I_DISCO
- Hỗ trợ màn hình LCD, cảm ứng, expander I/O và các module ngoại vi trên board

### 4.2 Drivers/CMSIS
- Thư viện CMSIS cho ARM Cortex-M4
- Cung cấp các macro, header và API cho nvic, system timer, core register

### 4.3 Drivers/STM32F4xx_HAL_Driver
- Driver HAL chuẩn của ST cho STM32F4
- Là lớp trừu tượng dùng để điều khiển GPIO, UART, SPI, I2C, DMA, timer, LTDC, FMC...

Nhìn chung, thư mục Drivers thực hiện vai trò: làm cầu nối giữa firmware và phần cứng thật của board.

---

## 5. Thư mục TouchGFX

Đây là phần giao diện người dùng của project.

### 5.1 TouchGFX/App
- app_touchgfx.c và app_touchgfx.h
  - Là lớp bridge giữa STM32Cube/firmware và TouchGFX runtime
  - Cung cấp các hàm khởi tạo và chạy task giao diện

### 5.2 TouchGFX/gui
Chứa toàn bộ giao diện người dùng theo mô hình MVVM của TouchGFX.

- gui/include/gui/model/Model.hpp
  - Định nghĩa lớp Model cho ứng dụng
  - Quản lý danh sách thiết bị, thiết bị đang được chọn, trạng thái upload, nút đang học

- gui/src/model/Model.cpp
  - Implement logic nghiệp vụ của GUI
  - Kết nối với thư viện IR C ở tầng firmware
  - Khi người dùng nhấn nút học, nó nhận tín hiệu từ tầng đầu vào IR và lưu vào device
  - Khi người dùng bấm nút phát, nó gọi hàm phát tín hiệu IR

- gui/include/gui/tv_screen_screen/ và gui/src/tv_screen_screen/
  - Màn hình remote cho TV
  - Presenter/View xử lý việc bấm nút như POWER, VOL+, CH+, ...
  - Presenter chuyển yêu cầu từ UI sang Model

- gui/include/gui/ac_screen_screen/ và gui/src/ac_screen_screen/
  - Màn hình remote cho điều hòa/AC
  - Có logic tương tự TV nhưng dành cho nút AC

- gui/include/gui/add_device_screen/ và gui/src/add_device_screen/
  - Màn hình thêm thiết bị mới vào danh sách
  - Cho phép tạo device mới khỏi UI

- gui/include/gui/common/, gui/src/common/
  - Thành phần UI chung dùng cho nhiều màn hình

- gui/include/gui/containers/, gui/src/containers/
  - Các container UI (ví dụ danh sách thiết bị, khung nút, view component)

### 5.3 TouchGFX/assets
- Chứa font, hình ảnh, text resource dùng cho giao diện
- Dùng để vẽ nút bấm, icon, background cho màn hình remote

### 5.4 TouchGFX/target
- Chứa mã nền cho mục tiêu board STM32F429I_DISCO
- Dùng cho việc tích hợp LCD, touch controller, hệ thống đồ họa vào MCU

### 5.5 TouchGFX/simulator
- Chứa môi trường giả lập chạy trên PC để test giao diện mà không cần board thật

### 5.6 TouchGFX/DieuKhienDaNang.touchgfx
- File thiết kế giao diện TouchGFX
- Là file project UI cho phần thiết kế hình ảnh và layout màn hình

---

## 6. Thư mục Middlewares

Chứa các thư viện và framework bên thứ ba dùng cho project.

- Middlewares/ST/touchgfx
  - Runtime và thư viện TouchGFX

- Third_Party/FreeRTOS
  - Source code FreeRTOS dùng cho task scheduling

Những thư mục này không phải code do project viết trực tiếp, mà là nền tảng chạy cho firmware và GUI.

---

## 7. Các thư mục build và IDE

### 7.1 EWARM
- Project dành cho IAR Embedded Workbench
- Chứa file cấu hình build, linker, startup cho compiler IAR

### 7.2 MDK-ARM
- Project dành cho Keil MDK-ARM
- Chứa file .uvprojx, .sct, cấu hình linker cho ARM Compiler

### 7.3 STM32CubeIDE
- Project cho STM32CubeIDE
- Dùng cho phát triển và debug bằng môi trường Eclipse/STM32Cube

### 7.4 gcc
- Chứa Makefile, linker script và các script build dùng GCC
- Dùng khi build bằng toolchain GNU

---

## 8. Luồng hoạt động chính của project

Hiểu ngắn gọn, project hoạt động theo chuỗi sau:

1. Khi boot, firmware khởi tạo các peripheral cần thiết
   - GPIO, UART, timer, LCD, TouchGFX, SDRAM

2. Timer TIM2 bắt tín hiệu IR từ đầu thu
   - Mỗi cạnh lên/xuống được ghi nhận như một thời điểm
   - Dựa trên khoảng thời gian giữa các cạnh, firmware thu được chuỗi mark/space

3. Khi một khung tín hiệu hoàn tất, firmware giải mã
   - Nếu là NEC/Sony/RC5, decode thành address/command
   - Nếu là tín hiệu phức tạp, lưu ở dạng RAW

4. Tầng GUI nhận dữ liệu này qua biến chung
   - Nếu người dùng đang ở chế độ học lệnh, tín hiệu mới sẽ được gắn cho nút đang chọn

5. Khi người dùng bấm nút trên màn hình
   - Nếu đang ở chế độ học: hệ thống ghi nhận nút cần học và chờ tín hiệu IR mới
   - Nếu không: hệ thống phát lại tín hiệu IR tương ứng đã lưu trước đó

---

## 9. Tóm tắt vai trò từng nhóm file

- Core/Inc: header và config
- Core/Src: logic thực thi firmware
- Drivers: driver/hardware abstraction
- TouchGFX/gui: UI và flow nghiệp vụ màn hình
- TouchGFX/assets: hình ảnh/font/resource UI
- TouchGFX/target: platform-specific rendering integration
- Middlewares: thư viện nền tảng
- EWARM/MDK-ARM/STM32CubeIDE/gcc: project build setup

---

## 10. Nếu bạn muốn hiểu nhanh nhất

Nếu chỉ muốn nắm “đường đi” chính của hệ thống, hãy đọc theo thứ tự sau:

1. Core/Src/main.c
2. Core/Src/ir_signal.c
3. Core/Src/ir_protocols.c
4. Core/Src/ir_device.c
5. TouchGFX/gui/src/model/Model.cpp
6. TouchGFX/gui/src/tv_screen_screen/TV_ScreenPresenter.cpp
7. TouchGFX/gui/src/ac_screen_screen/AC_ScreenPresenter.cpp

Đây là các file cốt lõi để hiểu toàn bộ logic ứng dụng.

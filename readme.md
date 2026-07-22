# VersatileRemote - Smart Remote Control with STM32 & TouchGFX

Dự án này là hệ thống điều khiển từ xa vạn năng (Versatile Remote) chạy trên board mạch **STM32F429I_DISCO** kết hợp giao diện đồ họa **TouchGFX**. Hệ thống hỗ trợ tính năng học lệnh (Learn) tín hiệu hồng ngoại (IR) từ các remote thực tế và phát lại (Playback) để điều khiển thiết bị thật (như TV, Điều hòa/AC).

Tài liệu này tổng hợp toàn bộ các tính năng lớn đã được phát triển và hoàn thiện trên nhánh `Phuong-MT`.

---

## Các tính năng đã triển khai

### 1. Thu nhận & Giải mã Tín hiệu Hồng ngoại (IR Capture & Decoding)
- **Thu nhận xung nhịp**: Sử dụng Timer `TIM2` cấu hình ở chế độ Input Capture (phát hiện cả 2 cạnh lên/xuống - Both Edge) trên chân thu tín hiệu để đo thời lượng Mark (có sóng mang) và Space (khoảng lặng) của tín hiệu IR với độ chính xác micro-giây.
- **Giải mã giao thức**:
  - Hỗ trợ giải mã tự động các giao thức phổ biến: **NEC**, **NEC Extended**, **Sony SIRC** (12-bit, 15-bit, 20-bit), và **Philips RC5**.
  - Đối với các tín hiệu không thuộc các giao thức trên (ví dụ: tín hiệu dài và phức tạp của Remote Điều hòa), hệ thống tự động lưu trữ dưới dạng chuỗi xung thô (**RAW**).
- **Lọc nhiễu thông minh**: Tự động loại bỏ các khung truyền quá ngắn do nhiễu môi trường (ít hơn 10 sườn xung) trong lúc học lệnh, giúp tránh việc học sai tín hiệu khởi tạo.

### 2. Tối ưu hóa Bộ nhớ & Sửa lỗi Stack Overflow
- **Tối ưu bộ nhớ SRAM**:
  - Giới hạn số lượng nút tối đa của mỗi thiết bị (`IR_MAX_BUTTONS`) xuống 12 nút để tiết kiệm dung lượng RAM.
  - Loại bỏ hoàn toàn việc cấp phát bộ nhớ động (malloc). Chuyển sang sử dụng cơ chế cấp phát tĩnh (`Static Pool`) thông qua mảng quản lý Registry của thiết bị trong file `ir_device.c`.
- **Khắc phục lỗi FreeRTOS**: Khắc phục lỗi tràn ngăn xếp (Stack Overflow) bằng cách chuyển các biến đệm chứa tín hiệu hồng ngoại lớn (cấu trúc `ir_signal_t`) ra khỏi bộ nhớ Stack của Task và chuyển thành các biến tĩnh (Static) hoặc toàn cục (Global).

### 3. Phát lại Tín hiệu Hồng ngoại (IR Transmission)
- **Điều chế sóng mang**: Sử dụng chân GPIO `PD12` cấu hình Push-Pull tần số cao để phát tín hiệu.
- **Hiệu chuẩn phần mềm (Calibration)**:
  - Tần số sóng mang được phát sóng ở tần số chuẩn **38.4 kHz** (chu kỳ 26us).
  - Đã căn chỉnh thời gian trễ thực thi của tập lệnh phần cứng và câu lệnh đọc thanh ghi từ `13us` xuống **`11us`** trong hàm `ir_tx_carrier`, đảm bảo chu kỳ thực tế phát ra đầu phát đạt chính xác `13us` (tần số 38.4kHz chuẩn), tối ưu hóa cự ly truyền.
- **Tuân thủ chuẩn phát lặp (Sony Repeat)**:
  - Đối với các giao thức của Sony (`SONY_12`, `SONY_15`, `SONY_20`), tín hiệu phát được cấu hình phát lặp **3 lần liên tiếp** với khoảng trễ **45ms** giữa các khung để vượt qua mạch lọc nhiễu của thiết bị thật (như TV Sony).

### 4. Giao diện Người dùng & Luồng Học lệnh (TouchGFX Integration)
- **Giao tiếp liên Task**: Chia sẻ tín hiệu hồng ngoại bắt được từ Task hệ thống sang Task GUI của TouchGFX bằng các biến liên kết `extern "C"` volatile an toàn (`g_gui_ir_frame_ready`, `g_gui_ir_frame`).
- **Luồng học lệnh (`upload_signal`)**:
  - Khi nhấn nút `upload_signal`, giao diện chuyển sang chế độ nạp tín hiệu.
  - **Chỉ thị trạng thái bằng màu sắc động**:
    - Màu **Vàng (Waiting)**: Nút bấm sẵn sàng chờ người dùng chọn để nạp tín hiệu.
    - Màu **Đỏ (Learning)**: Nút được chọn đang lắng nghe tín hiệu IR.
    - Màu **Xanh lá (Success)**: Nút `upload_signal` phát sáng báo hiệu đang hoạt động hoặc hoàn thành học lệnh thành công.
  - **Xử lý đặc biệt nút Power**: Do nút Power có hình ảnh màu đỏ mặc định, trạng thái chờ (Waiting) sẽ đổi màu nền sang màu Đen và viền Vàng; trạng thái học (Learning) sẽ hiển thị viền Xanh lá để đồng bộ chỉ thị màu sắc với các nút khác.
  - **Cơ chế dọn sạch bộ đệm (`ir_receive_flush`)**: Ngay khi nhấn chọn một nút để học lệnh, hệ thống tự động dọn sạch bộ đệm nhận của chip và xóa cờ tín hiệu cũ để đảm bảo không bị "nuốt" hay nạp nhầm tín hiệu của lần bấm trước đó.
- **Trải nghiệm người dùng tốt hơn**:
  - Tự động hủy chế độ học lệnh khi người dùng chuyển thiết bị hoặc chuyển màn hình (`deactivatePresenter`).
  - Tự động gán thiết bị đầu tiên được tìm thấy làm thiết bị hoạt động mặc định (`activeDevice`) ngay khi tải trang, giúp người dùng có thể thực hiện học lệnh/phát lệnh ngay mà không bắt buộc phải nhấn chọn thiết bị thủ công.

### 5. Giám sát Hệ thống (UART Debug Logging)
- Cấu hình cổng UART `USART1` (Baudrate: 115200, 8-N-1) để in ra các thông tin chẩn đoán chi tiết:
  - Khi bắt đầu chế độ upload, đăng ký học nút bấm.
  - Thông số kỹ thuật của tín hiệu học thành công (Giao thức, Địa chỉ, Mã lệnh Command, Số bit, Độ dài chuỗi RAW).
  - Chi tiết quá trình phát lại tín hiệu (Phát lại nút nào, thiết bị nào, tần số/chu kỳ sóng mang và mảng xung nhịp chi tiết).

## Cấu hình chân ngoại vi (Peripheral Pin Configuration)

Để hệ thống hoạt động chính xác, các linh kiện ngoại vi thu/phát hồng ngoại cần được kết nối vào các chân GPIO của mạch STM32F429I_DISCO theo cấu hình dưới đây:

| Linh kiện ngoại vi | Chân GPIO trên Board | Chức năng cấu hình trong vi điều khiển | Ghi chú |
| :--- | :--- | :--- | :--- |
| **Mắt thu hồng ngoại (IR Receiver)** | **`PA5`** | `TIM2_CH1` (Input Capture mode) | Thu nhận xung và đo thời gian Mark/Space |
| **Đầu phát hồng ngoại (IR Transmitter)** | **`PD12`** | `GPIO_MODE_OUTPUT_PP` (Push-Pull, High Speed) | Phát xung sóng mang 38kHz điều chế |
| **Debug Console (UART)** | Cổng **`USART1`** | Baudrate `115200`, 8-N-1 | In thông tin chẩn đoán và log hệ thống |

---

## Hướng dẫn Kiểm thử (Testing Guide)

1. **Chuẩn bị**:
   - Kết nối cổng thu hồng ngoại vào chân Input Capture của chip.
   - Kết nối module phát hồng ngoại `IR Transmitter` (chân dữ liệu SIG kết nối vào chân `PD12` của STM32, cấp nguồn `VCC` vào chân `5V`).
   - Mở terminal giám sát UART qua cổng `USART1` trên máy tính:
     ```bash
     pio device monitor -p /dev/cu.usbmodem103 -b 115200
     ```

2. **Quy trình học lệnh và phát**:
   - Trên màn hình điều khiển (TV hoặc AC), hệ thống đã chọn sẵn thiết bị mặc định.
   - Nhấn nút **Upload Signal** trên màn hình -> Các phím chức năng đổi sang màu **Vàng**.
   - Nhấn một nút cần học (ví dụ phím **VOL+**) -> Phím đổi sang màu **Đỏ**.
   - Hướng remote thật vào mắt thu hồng ngoại và bấm nút tương ứng -> Terminal hiển thị thông tin học thành công (`Model: SUCCESSFULLY LEARNED button 'VOL+'...`), phím chức năng chuyển về màu **Vàng**.
   - Nhấn lại nút **Upload Signal** để thoát chế độ học -> Các nút chức năng chuyển về màu mặc định.
   - Bấm nút vừa học trên màn hình giao diện -> Terminal in ra log bắt đầu phát (`Model: Transmitting button...`) và thiết bị thật (TV/AC) sẽ phản hồi lại lệnh tương ứng.
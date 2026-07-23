# BÁO CÁO BÀI TẬP LỚN

**Môn học:** IT4210 – Hệ nhúng

**ĐỀ TÀI:** XÂY DỰNG HỆ THỐNG ĐIỀU KHIỂN TỪ XA VẠN NĂNG (VERSATILEREMOTE) TRÊN KIT STM32F429

**Sinh viên thực hiện:**

| STT | Họ và tên | Mã số sinh viên | Nội dung thực hiện |
| :---: | :--- | :---: | :--- |
| 1 | Vũ Minh Ánh | 20225258 | Triển khai và tích hợp tính năng trigger mô-tơ rung phản hồi khi học tín hiệu thành công |
| 2 | Trịnh Tuấn Phong | 20225220 | Xây dựng giao diện |
| 3 | Lê Đức Phương | 20225380 | Xây dựng luồng logic, state, function các button trong giao diện |
| 4 | Trương Minh Phương | 20225381 | Triển khai logic thu nhận, giải mã và phát lại tín hiệu hồng ngoại (IR driver) |
| 5 | Trần Đức Thu | 20225412 | Triển khai logic thu nhận, giải mã và phát lại tín hiệu hồng ngoại (IR driver) |

---

## 1. GIỚI THIỆU ĐỀ TÀI

Dự án này là hệ thống điều khiển từ xa vạn năng (Versatile Remote) chạy trên board mạch **STM32F429I_DISCO** kết hợp giao diện đồ họa **TouchGFX**. 

Hệ thống hỗ trợ tính năng học lệnh tín hiệu hồng ngoại (IR) từ các remote thực tế thông qua việc thu nhận, giải mã, đo đạc vi giây thời lượng sóng mang. Sau đó, hệ thống lưu trữ và phát lại chính xác các tín hiệu này để điều khiển trực tiếp thiết bị thật (như TV, Điều hòa/AC). Dự án tập trung vào việc giải quyết các bài toán kỹ thuật thực tế trên vi điều khiển như tối ưu hóa bộ nhớ SRAM, sửa lỗi tràn ngăn xếp của FreeRTOS, và cấu hình hiệu chuẩn phần mềm để phát sóng mang tần số chuẩn 38.4 kHz.

## 2. KIẾN TRÚC HỆ THỐNG

### 2.1 Quy trình học và phát lệnh 

1. **Khởi động:** Hệ thống khởi tạo và gán một thiết bị hoạt động mặc định (`activeDevice`) ngay khi tải trang.
2. **Kích hoạt học lệnh (Learning):**
   * Nhấn nút **Upload Signal** trên màn hình giao diện TouchGFX.
   * Các nút chức năng trên màn hình điều khiển chuyển sang màu **Vàng (Waiting)**.
   * Nhấp chọn nút cần học (ví dụ: nút **VOL+**) -> Nút đó chuyển sang màu **Đỏ (Learning)**. Đồng thời hệ thống xóa bộ đệm cũ và sẵn sàng lắng nghe tín hiệu từ mắt thu IR.
3. **Nạp tín hiệu:**
   * Hướng remote thật vào mắt thu hồng ngoại (`PA5`) và bấm nút tương ứng.
   * Khi nhận tín hiệu hợp lệ (vượt qua bộ lọc nhiễu 10 xung), hệ thống giải mã, lưu tín hiệu vào Registry tĩnh.
   * Mô-tơ rung kết nối vào chân `PE6` sẽ **rung phản hồi một nhịp ngắn** để báo hiệu học thành công.
   * Nút trên màn hình chuyển lại sang màu **Vàng (Waiting)**. Nhấn lại nút **Upload Signal** lần nữa để thoát chế độ học lệnh (các nút trở về màu mặc định).
4. **Phát lệnh (Playback):**
   * Bấm nút vừa học trên màn hình TouchGFX.
   * Hệ thống truy xuất mã tín hiệu đã lưu và phát lại qua đầu phát hồng ngoại ở chân `PD12`.

### 2.2 Cấu hình chân ngoại vi 
Để hệ thống hoạt động chính xác, các linh kiện ngoại vi thu/phát hồng ngoại cần được kết nối vào các chân GPIO của mạch STM32F429I_DISCO theo cấu hình dưới đây:

| Linh kiện ngoại vi | Chân GPIO trên Board | Chức năng cấu hình trong vi điều khiển | Ghi chú |
| :--- | :--- | :--- | :--- |
| **Mắt thu hồng ngoại (IR Receiver)** | **`PA5`** | `TIM2_CH1` (Input Capture mode) | Thu nhận xung và đo thời gian Mark/Space |
| **Đầu phát hồng ngoại (IR Transmitter)** | **`PD12`** | `GPIO_MODE_OUTPUT_PP` (Push-Pull, High Speed) | Phát xung sóng mang 38.4kHz điều chế |
| **Mô-tơ rung (Vibration Motor)** | **`PE6`** | `GPIO_MODE_OUTPUT_PP` | Rung phản hồi khi học tín hiệu thành công |
| **Debug Console (UART)** | Cổng **`USART1`** | Baudrate `115200`, 8-N-1 | In thông tin chẩn đoán và log hệ thống |

---

## 3. THIẾT KẾ PHẦN MỀM

### 3.1 Thu nhận & Giải mã Tín hiệu Hồng ngoại (IR Capture & Decoding)
  * Hỗ trợ giải mã tự động các giao thức phổ biến: **NEC**, **Samsung 32-bit (hỗ trợ địa chỉ 16-bit đầy đủ)**, **Sony SIRC** (12-bit, 15-bit, 20-bit), và **Philips RC5**.
  * Đối với các tín hiệu không thuộc các giao thức trên (ví dụ: tín hiệu của Remote Điều hòa), hệ thống tự động lưu trữ dưới dạng chuỗi xung thô (**RAW**).
  * **Lọc nhiễu thông minh:** Tự động loại bỏ các khung truyền quá ngắn do nhiễu môi trường (ít hơn 10 sườn xung) trong lúc học lệnh, giúp tránh học sai tín hiệu.


### 3.2 Phát lại Tín hiệu Hồng ngoại (IR Transmission)
  * Sử dụng chân GPIO điều khiển LED hồng ngoại điều chế tần số sóng mang **38.4 kHz** với độ chính xác cao nhờ hiệu chuẩn trễ phần cứng bằng phần mềm.
  * Hỗ trợ phát lặp tự động cho giao thức Sony (phát lặp 3 lần với khoảng trễ 45ms) đảm bảo đầu thu nhận được lệnh chính xác.

### 3.3 Quản lý thiết bị lưu trữ dạng tĩnh (Static Registry)
  * Quản lý danh sách thiết bị và nút nhấn hoàn toàn bằng cơ chế tĩnh (`Static Pool`), không sử dụng bộ nhớ động (`malloc`) giúp loại bỏ hoàn toàn rủi ro phân mảnh bộ nhớ và lỗi FreeRTOS Stack Overflow.

### 3.4 Giao diện TouchGFX trực quan & phản hồi trạng thái bằng màu sắc động
  * Chỉ thị trạng thái bằng màu sắc của các nút nhấn trong quá trình nạp tín hiệu: Màu **Vàng (Waiting)** báo hiệu chờ chọn nút nạp lệnh, màu **Đỏ (Learning)** báo hiệu đang chờ nhận tín hiệu hồng ngoại.
  * **Phản hồi xúc giác (Haptic Feedback):** Kích hoạt mô-tơ rung phản hồi bằng một nhịp rung ngắn ngay khi học thành công tín hiệu và lưu vào cơ sở dữ liệu.

---

## 4. KẾT QUẢ ĐẠT ĐƯỢC

### 4.1 Hướng dẫn Kiểm thử (Testing Guide)

1. **Chuẩn bị**:
   - Kết nối cổng thu hồng ngoại vào chân Input Capture của chip.
   - Kết nối module phát hồng ngoại `IR Transmitter` (chân dữ liệu SIG kết nối vào chân `PD12` của STM32, cấp nguồn `VCC` vào chân `5V`).
   - Mở terminal giám sát UART qua cổng **USART1** để theo dõi log hệ thống. Có hai cách thực hiện:

      **Cách 1 (Khuyến nghị trên Windows):**  
      Sử dụng phần mềm **Hercules Setup Utility**.
      - Chọn đúng cổng COM của STM32.
      - Cấu hình Baudrate: **115200**
      - Sau khi kết nối, toàn bộ thông tin debug sẽ được hiển thị trên cửa sổ Serial.

      **Cách 2 (Sử dụng PlatformIO Core):**  
      Nếu chưa có công cụ dòng lệnh của PlatformIO, cài đặt **PlatformIO Core (CLI)** hoặc cài đặt PlatformIO thông qua Visual Studio Code. Sau đó mở terminal và chạy:

      ```bash
      pio device monitor -p COM3 -b 115200
      ```

      Trong đó:
      - `pio`: chương trình dòng lệnh của PlatformIO.
      - `device monitor`: mở trình giám sát cổng Serial.
      - `-p COM3`: chỉ định cổng Serial cần kết nối (trên Windows thay `COM3` bằng cổng COM thực tế, trên Linux/macOS có thể là `/dev/ttyUSB0`, `/dev/ttyACM0`, `/dev/cu.usbmodem103`, ...).
      - `-b 115200`: thiết lập tốc độ truyền (baudrate) là **115200 bps**, phải trùng với cấu hình của `USART1` trong chương trình.

2. **Quy trình học lệnh và phát**:
   - Trên màn hình điều khiển (TV hoặc AC), hệ thống đã chọn sẵn thiết bị mặc định.
   - Nhấn nút **Upload Signal** trên màn hình -> Các phím chức năng đổi sang màu **Vàng**.
   - Nhấn một nút cần học (ví dụ phím **VOL+**) -> Phím đổi sang màu **Đỏ**.
   - Hướng remote thật vào mắt thu hồng ngoại và bấm nút tương ứng -> Terminal hiển thị thông tin học thành công (`Model: SUCCESSFULLY LEARNED button 'VOL+'...`), phím chức năng chuyển về màu **Vàng**.
   - Nhấn lại nút **Upload Signal** để thoát chế độ học -> Các nút chức năng chuyển về màu mặc định.
   - Bấm nút vừa học trên màn hình giao diện -> Terminal in ra log bắt đầu phát (`Model: Transmitting button...`) và thiết bị thật (TV/AC) sẽ phản hồi lại lệnh tương ứng.

### 4.2 Video và Hình ảnh thực tế

Video thử nghiệm với TV samsung: https://drive.google.com/file/d/1NFpHnL-IksqlOQEwt3tqpY8J2WS9AMIx/view?usp=sharing


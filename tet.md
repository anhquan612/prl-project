# Lý thuyết môn Kiến trúc máy tính - HUS 2021-2022

## 1. Nguyên tắc chuyển đổi một số từ cơ số này sang cơ số khác

### Biểu diễn tổng quát

Một số được biểu diễn dưới dạng tổng quát như sau:

<img src="https://latex.codecogs.com/svg.image?V_k&space;=&space;\sum_{i=-m}^{n-1}b_i&space;\times&space;k^i&space;" title="https://latex.codecogs.com/svg.image?V_k = \sum_{i=-m}^{n-1}b_i \times k^i " />
trong đó,

- V_k là số cần biểu diễn
- n là số lượng các chữ số phần nguyên (thứ tự các chữ số phần nguyên là từ n-1 đến 0, theo chiều từ trái qua phải)
- m là số lượng các chữ số phần lẻ (thứ tự các chữ số phần lẻ là từ -1 đến -m, theo chiều từ trái qua phải)
- b_i là giá trị của chữ số thứ i
- k là hệ cơ số

### Chuyển đổi từ hệ 10 sang hệ cơ số bất kỳ

Xét một số N trong hệ 10, để chuyển đổi nó sang hệ cơ số khác (tạm gọi là k), ta đơn giản là lần lượt chuyển đổi phần nguyên và phần lẻ của N sang hệ k.

Ta ký hiệu như sau:

- P: Phần nguyên
- Q: Phần lẻ (có dạng 0.xyz)
- Pk: Kết quả thu được sau khi chuyển đổi phần nguyên sang hệ k
- Qk: Kết quả thu được sau khi chuyển đổi phần lẻ sang hệ k

#### Phần nguyên

Cách làm: ta chia P cho k đến khi P = 0, tại mỗi lần chia, ta thu được phần dư của phép chia và đặt nó vào đầu Pk.

Mã giả tựa python:

```python
while P != 0:
	residual = P % k # phần dư, bit được thêm vào
	Pk.add_to_head(residual)
	P //= k
```

#### Phần lẻ

Cách làm: ta nhân Q với k, với kết quả thu được:
- Phần nguyên được lấy làm bit nhị phân, nó được đặt vào cuối của Qk.
- Phần lẻ tiếp tục được nhân với 2, quá trình kết thúc khi phần lẻ của kết quả bằng 0.

Lưu ý rằng sẽ có những số mà ta tốn quá nhiều lần lặp hoặc không thể đạt được phần lẻ bằng 0. Với những trường hợp như vậy, ta phải giới hạn số chữ số.

Mã giả tựa python:

```python
iters = 20
for _ in range(iters):
	if Q == 0:
		break
	re = Q * k
	bit = floor(re) # làm tròn xuống
	Qk.add_to_end(bit)
	Q = re - ip # giữ lại phần lẻ
```

#### Ví dụ

Ta lấy ví dụ chuyển số 10.25 hệ 10 sang hệ 2 (nhị phân).

Với phần nguyên:

| P | P/k | R | Pk |
| -	| --- | - | -- |
| 10 | 10/2 = 5 | 0 | 0 |
| 5 | 5/2 = 2 | 1 | 10 |
| 2 | 2/2 = 1 | 0 | 010 |
| 1 | 1/2 = 0 | 1 | 1010 |

Với phần dư:

| Q | Qxk | ip | Qk |
| - | --- | -- | -- |
| 0.25 | 0.25x2 = 0.5 | 0 | 0 |
| 0.5 | 0.5x2 = 1.0 | 1 | 01 |

=> Biểu diễn nhị phân của 10.25 là 1010.01.

### Chuyển đổi từ hệ cơ số k1 sang hệ cơ số k2

Gọi số cần chuyển đổi là N1 hệ k1.

Bước 1: Tính giá trị hệ thập phân của N1 (dựa vào biểu diễn tổng quát), ta gọi giá trị này là N.\
Bước 2: Chuyển đổi N hệ 10 sang hệ k2 (dựa vào phương pháp chuyển đổi hệ 10 sang hệ cơ số khác mà ta đã nói ở trên).

## 2. Nêu nguyên tắc biểu diễn số nguyên có dấu

Sử dụng bit đầu tiên cho dấu của số.

- 0: số dương
- 1: số âm

Nếu số cần biểu diễn là số dương, ta biểu diễn nó như bình thường. Còn nếu số cần biểu diễn là số âm, ta sử dụng bù 2 để biểu diễn.

Sử dụng phương pháp bù 2 biểu diễn số -N, ta làm lần lượt các bước như sau:

- Bước 1: Chuyển N sang hệ nhị phân.
- Bước 2: Đảo tất cả các bit nhận được ở bước 1.
- Bước 3: Cộng thêm 1 vào kết quả nhận được ở bước 2.

Lưu ý rằng vì ta biểu diễn số âm nên bit đầu tiên là 1.

Ví dụ với số -14 với mẫu 8 bit:

- Bước 1: Tính được biểu diễn hệ nhị phân của 14 là: 0000 1110.
- Bước 2: Đảo bit: 1111 0001.
- Bước 3: Cộng thêm 1: 1111 0010.

=> Biểu diễn -14 trong hệ nhị phân là 1111 0010.

## 3. Khái niệm về thông tin, mã hóa và giải mã thông tin

### Thông tin

Thông tin là sự hiểu biết của con người về một thực thể nào đó, có thể thu thập, lưu trữ, xử lý được.

Có thể phân thông tin thành hai loại:

- Số
- Phi số

### Mã hóa thông tin

Mã hóa thông tin là phương pháp biến đổi thông tin từ dạng bình thường sang dạng thông tin không thể hiểu được nếu không có phương tiện giải mã.

Để máy tính có thể xử lý được, thông tin phải được mã hóa thành dạng nhị phân.

### Giải mã thông tin

Giải mã thông tin là phương pháp để đưa thông tin từ dạng đã được mã hóa thành dạng thông tin ban đầu, nó là quá trình ngược của mã hóa.

## 4. Thực hiện phép cộng 2 số nhị phân có dấu 8 bit

Quy tắc cộng bit:

- 0 + 0 = 0
- 0 + 1 = 1 + 0 = 1
- 1 + 1 = 10

Tính chất:

- Cộng 2 số khác dấu luôn cho ra kết quả đúng (vì phép cộng này không thể gây tràn số).
- Cộng 2 số cùng dấu, nếu
	- Kết quả cùng dấu với các số hạng thì kết quả là đúng.
	- Kết quả bị ngược dấu, kết quả bị sai do bị tràn số.
	
Tràn số xảy ra khi kết quả nằm ngoài đoạn biểu diễn: [-2^(n-1), 2^(n-1) + 1] với n là số bit biểu diễn

### Ví dụ với n = 8:

**Ví dụ 1.** Cộng 2 số cùng dấu

**Ví dụ 1.1.** Cộng 2 số dương: 28 và 45. Kết quả của phép tính này ở hệ thập phân là 73.

Biểu diễn nhị phân của 28 là: 0001 1100\
Biểu diễn nhị phân của 45 là: 0010 1101

Thực hiện phép cộng:

| STT chữ số | Phép tính | Kết quả | Nhớ |
| ---------- | --------- | ------- | --- |
| 8 | 0 + 1 | 1 |   |
| 7 | 0 + 0 | 0 |   |
| 6 | 1 + 1 | 0 | 1 |
| 5 | (1 + 1) + 1 | 1 | 1 |
| 4 | (1 + 0) + 1 | 0 | 1 |
| 3 | (0 + 1) + 1 | 0 | 1 |
| 2 | (0 + 0) + 1 | 1 |   |
| 1 | 0 + 0 | 0 |   |

=> Kết quả là 0100 1001. Chuyển 0100 1001 từ hệ 2 về hệ 10, ta được: 73.

**Ví dụ 1.2.** Cộng 2 số âm: -28 và -45. Kết quả của phép tính này ở hệ thập phân là -73.

Biểu diễn nhị phân (bù 2) của -28 là: 1110 0100\
Biểu diễn nhị phân (bù 2) của -45 là: 1101 0011

Thực hiện phép cộng:

| STT chữ số | Phép tính | Kết quả | Nhớ |
| ---------- | --------- | ------- | --- |
| 8 | 0 + 1 | 1 |   |
| 7 | 0 + 1 | 1 |   |
| 6 | 1 + 0 | 1 |   |
| 5 | 0 + 0 | 0 |   |
| 4 | 0 + 1 | 1 |   |
| 3 | 1 + 0 | 1 |   |
| 2 | 1 + 1 | 0 | 1 |
| 1 | (1 + 1) + 1 | 1 | 1 |

Ta thấy rằng phép tính đã kết thúc nhưng số 1 vẫn đang được nhớ, ta bỏ qua số này. Kết quả thu được: 1011 0111.

Xác thực kết quả:

Số này đang ở dạng bù 2, ta chuyển nó về dạng nhị phân thông thường bằng cách làm ngược lại phương pháp bù 2:

- Bước 1: Trừ 1011 0111 đi 1, ta được 1011 0110.
- Bước 2: Đảo bit, thu được 0100 1001.

Chuyển kết quả thu được ở bước 2 về hệ thập phân, ta được kết quả là 73 => giá trị mà ta thu được sau khi cộng là -73 => Phép tính đúng.

**Ví dụ 1.3.** Cộng nhưng bị tràn số: 228 và 245. Kết quả của phép tính này ở hệ thập phân là 473.

Biểu diễn nhị phân của 228 là: 1110 0100\
Biểu diễn nhị phân của 245 là: 1111 0101

Thực hiện phép cộng:

| STT chữ số | Phép tính | Kết quả | Nhớ |
| ---------- | --------- | ------- | --- |
| 8 | 0 + 1 | 1 |   |
| 7 | 0 + 0 | 0 |   |
| 6 | 1 + 1 | 0 | 1 |
| 5 | (0 + 0) + 1 | 1 |   |
| 4 | 0 + 1 | 1 |   |
| 3 | 1 + 1 | 0 | 1 |
| 2 | (1 + 1) + 1 | 1 | 1 |
| 1 | (1 + 1) + 1 | 1 | 1 |

Vì chỉ có 8 bit để biểu diễn nên bit 1 được nhớ cuối cùng sẽ không nằm trong kết quả. Kết quả thu được: 1101 1001. Hệ thập phân của dãy này là 217 => Kết quả sai.

Nếu ta sử dụng mẫu nhiều hơn 8 thì bit nhớ cuối cùng là 1 được thêm vào kết quả và kết quả sẽ là 0001 1101 1001 (với mẫu 12 bit). Hệ thập phân của dãy này là 473 (kết quả đúng)

**Ví dụ 2.** Cộng hai số ngược dấu

**Ví dụ 2.1.** Cộng hai số đối nhau: 28 và -28. Kết quả của phép tính này luôn luôn là 0.

Biểu diễn nhị phân của 28 là: 0001 1100\
Biểu diễn nhị phân (bù 2) của -28 là: 1110 0100

Thực hiện phép cộng:

| STT chữ số | Phép tính | Kết quả | Nhớ |
| ---------- | --------- | ------- | --- |
| 8 | 0 + 0 | 0 |   |
| 7 | 0 + 0 | 0 |   |
| 6 | 1 + 1 | 0 | 1 |
| 5 | (1 + 0) + 1 | 0 | 1 |
| 4 | (1 + 0) + 1 | 0 | 1 |
| 3 | (0 + 1) + 1 | 0 | 1 |
| 2 | (0 + 1) + 1 | 0 | 1 |
| 1 | (0 + 1) + 1 | 0 | 1 |

Kết quả thu được: 0000 0000 => Kết quả đúng.

**Ví dụ 2.2.** |Số âm| < Số dương: -28 và 45. Kết quả của phép tính này trong hệ thập phân là 17.

Biểu diễn nhị phân của 45 là: 0010 1101\
Biểu diễn nhị phân (bù 2) của -28 là: 1110 0100

Thực hiện phép cộng:

| STT chữ số | Phép tính | Kết quả | Nhớ |
| ---------- | --------- | ------- | --- |
| 8 | 1 + 0 | 1 |   |
| 7 | 0 + 0 | 0 |   |
| 6 | 1 + 1 | 0 | 1 |
| 5 | (1 + 0) + 1 | 0 | 1 |
| 4 | (0 + 0) + 1 | 1 |   |
| 3 | 1 + 1 | 0 | 1 |
| 2 | (0 + 1) + 1 | 0 | 1 |
| 1 | (0 + 1) + 1 | 0 | 1 |

Kết quả thu được: 0001 0001. Hệ thập phân của dãy này là 17 => Đúng.

**Ví dụ 2.3.** |Số âm| > Số dương: -45 và 28. Kết quả của phép tính này trong hệ thập phân là -17.

Biểu diễn nhị phân của 28 là: 0001 1100\
Biểu diễn nhị phân (bù 2) của -45 là: 1101 0011

Thực hiện phép cộng:

| STT chữ số | Phép tính | Kết quả | Nhớ |
| ---------- | --------- | ------- | --- |
| 8 | 0 + 1 | 1 |   |
| 7 | 0 + 1 | 1 |   |
| 6 | 1 + 0 | 1 |   |
| 5 | 1 + 0 | 1 |   |
| 4 | 1 + 1 | 0 | 1 |
| 3 | (0 + 0) + 1 | 1 |   |
| 2 | 0 + 1 | 1 |   |
| 1 | 0 + 1 | 1 |   |

Kết quả thu được là 1110 1111. Thực hiện đảo ngược phép bù 2, ta thu được dãy 0001 0001, giá trị hệ thập phân của dãy này là 17 => Kết quả của ta là chính xác.

## 5. Trình bày về biểu diễn số thực dấu phẩy tĩnh, lấy ví dụ với 16 bit

Biểu diễn dấu phẩy tĩnh là dấu phẩy sẽ được đặt ở một vị trí cố định để ngăn cách phần nguyên với phần lẻ.

Vấn đề với cách biểu diễn này là nó phụ thuộc vào phạm vi của kiểu số nguyên, mất độ chính xác và dễ bị tràn.

[Cách chuyển đổi số thực dấu phẩy tĩnh từ hệ 10 sang hệ nhị phân](#chuyển-đổi-từ-hệ-10-sang-hệ-cơ-số-bất-kỳ)

Ví dụ với 16 bit: số 28.45

Phần nguyên:

| Phần nguyên/2 | Kết quả | Dư |
| ------------- | ------ | -- |
| 28/2          | 14     | 0  |
| 14/2          | 7      | 0  |
| 7/2      		| 3      | 1  |
| 3/2      		| 1      | 1  |
| 1/2      		| 0      | 1  |

=> Phần nguyên: 0000000000011100

Phần lẻ:

| Phần lẻ x 2 | Kết quả | Bit thu được |
| ----------- | ------- | ------------ |
| 0.45 x 2    | 0.9     | 0            |
| 0.9  x 2    | 1.8     | 1            |
| 0.8  x 2    | 1.6     | 1            |
| 0.6  x 2    | 1.2     | 1            |
| 0.2  x 2    | 0.4     | 0            |
| 0.4  x 2    | 0.8     | 0            |
| 0.8  x 2    | 1.6     | 1            |
| 0.6  x 2    | 1.2     | 1            |
| 0.2  x 2    | 0.4     | 0            |
| 0.4  x 2    | 0.8     | 0            |
| 0.8  x 2    | 1.6     | 1            |
| 0.6  x 2    | 1.2     | 1            |
| 0.2  x 2    | 0.4     | 0            |
| 0.4  x 2    | 0.8     | 0            |
| 0.8  x 2    | 1.6     | 1            |
| 0.6  x 2    | 1.2     | 1            |

=> Phần lẻ: 0111001100110011

=> 28.45 hệ nhị phân là: 0000000000011100.0111001100110011

## 6. Trình bày về biểu diễn số thực dấu phẩy động 32 bit, lấy ví dụ

Một số thực được biểu diễn dưới dạng dấu phẩy động 32 bit như sau:

- Phần dấu (S): 1 bit
- Phần mũ (E): 8 bit
- Phần lẻ (F): 23 bit

![](https://media.geeksforgeeks.org/wp-content/uploads/Single-Precision-IEEE-754-Floating-Point-Standard.jpg "Nguồn ảnh: GeeksforGeeks")

Nguồn ảnh: GeeksforGeeks

F tương ứng với Mantissa (phần định trị)

Tương ứng với số thực:

<img src="https://latex.codecogs.com/svg.image?(-1)^S&space;\times&space;(1&plus;F)&space;\times&space;2^{E-bias}" title="https://latex.codecogs.com/svg.image?(-1)^S \times (1+F) \times 2^{E-bias}" />

vì ta đang dùng 32 bit nên bias sẽ bằng 127.

Ví dụ: 

**Ví dụ 1.** Biểu diễn số -13.375

- Dấu âm => S = 1
- 13 sang hệ nhị phân là 1101
- 0.375 sang hệ nhị phân là 0.011

<img src="https://latex.codecogs.com/svg.image?{-13.375}_{10}&space;=&space;{-1101.011}_{2}&space;=&space;-1.101011&space;\times&space;2^3&space;=&space;1&space;10000010&space;10101100000000000000000" title="https://latex.codecogs.com/svg.image?{-13.375}_{10} = {-1101.011}_{2} = -1.101011 \times 2^3 = 1 10000010 10101100000000000000000" />

Giải thích:

- S = 1
- E = 10000010 (E = 3 + 127 = 130, chuyển 130 sang hệ nhị phân, ta được kết quả này)
- F = 101011 và ta chèn thêm các bit 0 vào sau để đủ 23 bit

## 7. Trình bày các thành phần cơ bản của máy tính

Các thành phần cơ bản của máy tính bao gồm:

- Bộ xử lý trung tâm (CPU)
- Bộ nhớ máy tính
- Hệ thống input - output

### Bộ xử lý trung tâm (CPU)

Chức năng: Là nơi xử lý dữ liệu và điều khiển hoạt động của máy tính.

Nguyên tắc hoạt động:

- CPU hoạt động theo chương trình nằm trong bộ nhớ chính bằng cách nhận lần lượt từng lệnh từ bộ nhớ này, giải mã rồi thực thi lệnh.
- Trong quá trình thực thi, CPU có thể trao đổi dữ liệu với bộ nhớ chính và hệ thống input - output.

Các thành phần cơ bản:

- Đơn vị điều khiển (CU): Điều khiển hoạt động của máy tính.
- Đơn vi số học và logic (ALU): Thực hiện tính toán số học và logic.
- Thanh ghi (RF): Lưu trữ thông tin tạm thời phục vụ cho hoạt động của CPU.
- Đơn vị ghép nối bus (BIU): Kết nối và trao đổi thông tin giữa các bus bên trong và bên ngoài.

### Bộ nhớ máy tính

Chức năng: Lưu trữ dữ liệu.

Phân loại:

- Bộ nhớ trong
	- Chứa dữ liệu mà CPU có thể truy cập trực tiếp
	- Tốc độ nhanh, dung lượng nhỏ
	- Sử dụng bộ nhớ bán dẫn RAM, ROM
	- Bao gồm:
		- Bộ nhớ chính: chứa các chương trình, dữ liệu đang được CPU sử dụng
		- Bộ nhớ đệm: dung lượng nhỏ, tốc độ nhanh hơn bộ nhớ chính
- Bộ nhớ ngoài
	- Tốc độ chậm, dung lượng lớn
	- Kết nối với hệ thống dưới dạng thiết bị input - output
	- Lưu trữ tài nguyên phần mềm
	- Nhiều loại:
		- Bộ nhớ từ: HDD, FDD,...
		- Bộ nhớ quang: đĩa CD, DVD,...
		- Bộ nhớ bán dẫn: SSD, thẻ nhớ,...
		
### Hệ thống input - output

Chức năng: Trao đổi thông tin giữa máy tính với bên ngoài.

Các thành phần chính:

- Các thiết bị ngoại vi: chuyển đổi dữ liệu giữa trong và ngoài máy tính.
	- Thiết bị input: chuột, bàn phím,...
	- Thiết bị output: màn hình, máy in, loa,...
	- Thiết bị lưu trữ: thẻ nhớ, SSD, HDD,...
- Các module input - output: nối ghép các thiết bị ngoại vi với máy tính.

## 8. Trình bày về bộ nhớ từ, so sánh HDD, FDD, băng từ

Đặc điểm:

- Bộ nhớ tĩnh điện
- Vật liệu khả từ
- Trạng thái từ tính biểu diễn các bit 0 và 1
- Nhược điểm:
	- Tuổi thọ ngắn
	- Tiếng ồn khi hoạt động
	- Tốc độ truy xuất dữ liệu bị giới hạn bởi tốc độ quay
	- Tỏa nhiều nhiệt lượng
- Ứng dụng:
	- Băng từ
	- Ổ đĩa từ

### Băng từ

Cấu tạo:

- Cuộn trục phim
- Phim nhựa mỏng, dài
- Vật liệu từ (thường là Fe203)	tráng trên mặt phim nhựa

Ưu điểm:

- Yêu cầu công nghệ thấp
- Chi phí thấp
- Khối lượng nhẹ
- Dùng chủ yếu cho lưu trữ nguội, chỉ có tác động vật lý mới ảnh hưởng đến dữ liệu
- Mật độ lưu trữ dữ liệu 3 chiều khá lớn (0.2 Tbit/in^3)

Nhược điểm:

- Mật độ lưu trữ dữ liệu 2 chiều thấp (3.1 Gbit/in^2)
- Dữ liệu dễ bị ảnh hưởng bởi tác động vật lý
- Cồng kềnh

### Ổ đĩa từ

Cấu tạo:

- Một đĩa tròn làm bằng vật liệu khả từ
- Đầu đọc/ghi chạy sát bề mặt đĩa

#### FDD (Floppy Disk Drive) vs HDD (Hard Disk Drive)

|   | FDD | HDD |
| - | --- | --- |
| Khối lượng | Nhẹ | Nặng |
| Mật độ lưu trữ dữ liệu | Thấp (1.44 Mb cho đĩa 3.5'') | Lớn (thông thường ~800 Gbit/in^2) |
| Giá thành | Đắt (so với HDD) | Rẻ (~$0.05/Gb) |
| Thời điểm thịnh hành | Thập niên 70 - đầu thế kỷ XXI | Đầu thế kỷ XXI |

## 9. Trình bày về bộ nhớ quang, so sánh CD, DVD, Blu-ray

### Ổ đĩa quang học

Đĩa cấu tạo gồm nhiều lớp được ép chặt vào nhau:

- Lớp nhựa có vai trò bảo vệ
- Lớp phản quang có vai trò lưu trữ dữ liệu

Ổ đĩa gồm:

- Motor đĩa quay
- Đầu đọc/ghi sử dụng tia laser

Nguyên lý hoạt động:

- Ghi dữ liệu: Máy tính dịch dữ liệu cần ghi thành các trạng thái phản xạ của mặt đĩa, sau đó dùng tia laser khắc các trạng thái lên mặt đĩa.
- Đọc dữ liệu: Máy tính chiếu tia laser vào mặt đĩa, ghi nhận các trạng thái phản xạ của ánh sáng, sau đó dịch thành mã nhị phân.

Ưu điểm:

- Kích thước nhỏ (đường kính 12cm, dày 1.2mm)
- Khối lượng nhẹ (15g)

Nhược điểm:

- Tiếng ồn khi hoạt động
- Tốc độ truy xuất bị giới hạn bởi tốc độ quay
- Mật độ dữ liệu phụ thuộc vào bước sóng của tia laser khi ghi
- Dễ bị hỏng do tác động vật lý

### CD vs DVD vs Blu-ray

|   | CD | DVD | Blu-ray |
| - | -- | --- | ------- |
| Năm sáng chế/công bố | 1970 | 1995 | 2006 |
| Bước sóng | 780nm | 650nm | 405nm |
| Dung lượng phổ biến | 700Mb | 4.7Gb | 25Gb |

## 10. Trình bày về bộ nhớ ROM, RAM (so sánh SRAM, DRAM)

ROM và RAM là các bộ nhớ chính của máy tính, được dùng để lưu trữ các chương trình và dữ liệu trong suốt quá trình hoạt động của máy tính.

### ROM (Read-Only Memory)

ROM là bộ nhớ không khả biến, tức là trong vận hành thông thường của hệ thống, dữ liệu chỉ có thể được đọc ra mà không thể ghi vào.

Đặc điểm:

- Được dùng để lưu trữ mã chương trình hệ điều hành và dữ liệu mặc định của hệ thống.
- Dữ liệu trong ROM không bị mất đi khi mất nguồn điện cung cấp.
- Các ROM thế hệ đầu chỉ cho phép ghi dữ liệu một lần duy nhất, gọi là nạp ROM.
- Các ROM thế hệ sau cho phép xóa dữ liệu và nạp ROM nhiều lần, tuy nhiên việc này vẫn phải tuân theo quy trình đặc biệt.

Phân loại:

- PROM (Programmable Read-Only Memory): Chỉ có thể được lập trình một lần duy nhất bằng các thiết bị đăc biệt.
- EPROM (Erasable Programmable Read-Only Memory): Có thể bị xóa bằng tia cực tím và ghi bằng thiết bị ghi EPROM.
- EAROM (Electrically Alterable Read-Only Memory): Có thể thay đổi theo từng bit tuy nhiên tốc độ ghi tương đối chậm.
- EEPROM (Electrically Erasable Programmable Read-Only Memory): Có thể xóa đi hoặc viết vào bằng điện cực.

### RAM (Random-Access Memory)

RAM là bộ nhớ khả biến.

Đặc điểm:

- Được dùng để lưu trữ tạm thời các chương trình, dữ liệu trong hoạt động của máy tính.
- Dữ liệu trong RAM bị mất đi khi mất nguồn điện cung cấp.
- Cho phép truy xuất đọc/ghi ngẫu nhiên đến bất kỳ vị trí nào trong bộ nhớ dựa theo địa chỉ bộ nhớ.

#### SRAM vs DRAM

SRAM (Static RAM - RAM tĩnh)

DRAM (Dynamic RAM - RAM động)

|   | SRAM | DRAM |
| - | ---- | ---- |
| Giá thành | Đắt | Rẻ |
| Mật độ | Thấp | Cao |
| Năng lượng tiêu tốn | Nhiều | Ít |
| Cần refresh? | Không | Có |
| Lưu trữ dữ liệu bằng | Bóng bán dẫn | Tụ điện |
| Dạng lưu trữ của các bit | Điện áp | Năng lượng điện |
| Tốc độ truy xuất | Nhanh (10ns - 20ns) | Chậm (60ns - 80ns) |

## 11. Trình bày về kỹ thuật ống dẫn, nêu các xung đột và phương pháp giải quyết

Kỹ thuật ống dẫn (pipeline) là kỹ thuật làm cho các giai đoạn khác nhau của nhiều lệnh được thực thi cùng 1 lúc.

Sử dụng kỹ thuật ống dẫn sẽ làm giảm thời gian hoàn thành tất cả công việc do các công việc được thực hiện song song.

Khi thực thi, các lệnh được chia thành 5 công đoạn:

1. Nạp lệnh từ bộ nhớ
2. Giải mã lệnh và đọc các thanh ghi
3. Thực thi các phép tính
4. Truy xuất bộ nhớ
5. Ghi kết quả vào thanh ghi

### Các xung đột và phương pháp giải quyết

Xung đột là trạng thái mà lệnh tiếp theo không thể thực thi trong chu kỳ pipeline ngay sau đó (hoặc thực thi nhưng cho ra kết quả)

- Xung đột cấu trúc: là khi một lệnh dự kiến không thể thực thi trong đúng chu kỳ pipeline của nó do phần cứng không thể hỗ trợ.
- Xung đột dữ liệu: là khi một lệnh dự kiến không thể thực thi trong đúng chu kỳ pipeline của nó do dữ liệu mà lệnh này cần chưa sẵn sàng.
- Xung đột điều khiển: là khi một lệnh dự kiến không thể thực thi trong đúng chu kỳ pipeline của nó do lệnh được nạp vào không phải là lệnh được cần.

Phương pháp giải quyết cho

- Xung đột cấu trúc: trang bị thêm nhiều bộ phận chức năng, cải thiện phần cứng.
- Xung đột dữ liệu: sử dụng kỹ thuật forwarding
	- Kỹ thuật forwarding: giải quyết xung đột dữ liệu bằng cách đưa thêm vào các bộ đệm phụ bên trong, các dữ liệu cần có thể được truy xuất từ bộ đệm này thay vì đợi đến khi nó sẵn sàng.
- Xung đột điều khiển:
	- Sử dụng phương pháp dự đoán xem lệnh nào sẽ được nạp. Nếu dự đoán đúng, chương trình sẽ không lãng phí chu kỳ xung clock nào. Ngược lại, nếu dự đoán sai, lệnh đúng sẽ được nạp lại và ta tốn một chu kỳ xung clock.	

## 12. Trình bày về máy tính song song

Máy tính song song là một tập các bộ xử lý (thường là cùng một loại) kết nối với nhau theo một cách nào đó mà có thể hợp tác với nhau cùng hoạt động và trao đổi dữ liệu với nhau.

Xử lý song song: Bài toán được tách thành nhiều phần và có thể xử lý đồng thời, mỗi phần tách thành các lệnh rời rạc, mỗi phần được thực hiện từ những CPU khác nhau.

Đặc điểm của bộ nhớ máy tính song song:

- Sử dụng nhiều thanh ghi: giảm hiệu ứng phụ của các ngắt.
- Không gian nhớ lớn: tăng hiệu quả trao đổi dữ liệu của hệ thống.
- Xây dựng bộ lập lịch (Scheduling): cấp phát hữu hiệu từng nhiệm vụ đơn lẻ cho các bộ xủ lý.
- Đồng bộ các bộ xử lý (Synchronization): điều khiển nhiều tiến trình hoạt động đồng thời, cùng truy cập đến một số hữu hạn các tài nguyên dùng chung, tránh được deadlock.

Các mô hình song song:

- SISD (Single Instruction Stream, Single Data Stream): Đơn luồng lệnh, đơn luồng dữ liệu.
- SIMD (Single Instruction Stream, Multiple Data Stream): Đơn luồng lệnh, đa luồng dữ liệu.
- MISD (Multiple Instruction Stream, Single Data Stream): Đa luồng lệnh, đơn luồng dữ liệu.
- MIMD (Multiple Instruction Stream, Multiple Data Stream): Đa luồng lệnh, đa luồng dữ liệu.

## 13. Trình bày về cảm biến hình ảnh

Cảm biến hình ảnh là thiết bị có chức năng chuyển đổi tín hiệu hình ảnh thu được thành tín hiệu điện.

### Thành phần cơ bản

Các bộ phận chính của cảm biến hình ảnh:

- Nguồn sáng riêng: cung cấp ánh sáng cho cảm biến, đảm bảo thiết bị có đủ độ sáng để ghi lại những hình ảnh chất lượng nhất.
- Thấu kính: Đưa hình ảnh tới chip xử lý ảnh.
- Chip xử lý ảnh: Chuyển đổi tín hiệu quang học thành tín hiệu analog.
	- CCD (Charge Coupled Device)
	- CMOS (Complementary Metal Oxide Semiconductor)
- Bộ chuyển đổi tín hiệu analog: Xử lý và biến đổi tín hiệu tương tự sang tín hiệu số.
- Vi xử lý: Phân tích và xử lý hình ảnh, sau đó dựa trên thông số được cài đặt từ trước để quyết định.
- Input - Output: Cung cấp kênh giao tiếp với các thiết bị khác.
- Các thiết bị ngoại vi kết nối điều khiển khác.

### Nguyên lý hoạt động

Dựa trên Hiệu ứng quang điện. Mỗi cảm biến hình ảnh bao gồm một bảng mạch nhỏ gồm các photodiode rất nhạy cảm với ánh sáng, mỗi photodiode khi bị ánh sáng tác động sẽ sinh ra một điện áp tỉ lệ thuận với cường độ ánh sáng mà nó nhận tác động.

- Không có ánh sáng tác động => Không tạo ra mức điện áp nào cả => Mức đen.
- Cường độ ánh sáng ở mức cao nhất => Mức điện áp rất lớn => Mức trắng.
- Nằm trong hai khoảng trên => Mức xám.

### Quá trình hoạt động

3 giai đoạn:

- Thu thập: Camera chụp ảnh đối tượng.
- Phân tích: Bức ảnh được lưu tại bộ nhớ, sau đó thực hiện xử lý, phân tích, so sánh với các thông số đặt trước.
- Xác định: Căn cứ vào dữ liệu thu thập, phân tích, cũng như thông số đặt trước để đưa ra kết quả ở đầu ra.

### Phân loại

Dựa trên cấu trúc, ta có 2 loại cảm biến hình ảnh phổ biến là:

- CCD
- CMOS

### CMOS

Được sử dụng phổ biến.

Sử dụng bộ lọc màu RGB sau đó chuyển sang dữ liệu dạng tín hiệu số rồi đưa vào thẻ nhớ để lưu lại.

### CCD

Thành phần quan trọng của cảm biến này là photodiode.

Nó hoạt động theo cơ chế của thanh ghi dịch mà nhờ đó thu được hình ảnh của một hàng mà không cần nhiều đầu dây nối.

###  CMOS vs CCD

|   | CMOS | CCD |
| - | ---- | --- |
| Kiểu màn trập | Rolling | Global |
| Độ nhiễu | Thấp | Rất thấp |
| Dải tương phản động | Cao | Rất cao |
| Tốc độ khung hình | Trung bình | Có thể đạt tới rất cao |
| Chi phí | Thấp | Cao |

## 14. Trình bày về chuẩn giao tiếp I2C

I2C (Inter-Integrated Circuit) là một giao thức giao tiếp được phát triển bởi Philips Semiconductors để truyền dữ liệu giữa một bộ xử lý trung tâm với nhiều IC trên cùng một board mạch, nó sử dụng 2 đường truyền tín hiệu.

Đây là một giao thức giao tiếp nối tiếp đồng bộ. Nghĩa là các bit dữ liệu được truyền từng bit một theo các khoảng thời gian đều đặn được thiết lập bởi một tín hiệu đồng hồ tham chiếu.

Hai đường truyền tín hiệu là:

- SDA (Serial Data): Đường truyền cho master và slave gửi và nhận dữ liệu.
- SCL (Serial Clock): Đường truyền mang tín hiệu đồng hồ.

Dữ liệu được truyền qua SDA và được đồng bộ với tín hiệu đồng hồ nhờ SCL.

### Thiết bị master và slave

Các thiết bị kết nối với bus I2C đều được phân loại là thiết bị master hoặc thiết bị slave.

Quy tắc:

- Tất cả thiết bị đáp ứng các chỉ dẫn từ thiết bị master thì đều là slave.
- Để phân biệt, mỗi thiết bị slave được gán một địa chỉ 7 bit cố định.
- Khi master muốn nhận/gửi dữ liệu từ một slave, nó xác định địa chỉ của slave đó trên đường truyền SDA và sau đó tiến hành truyền dữ liệu.
- Một slave chỉ được phép phản hồi khi master chỉ định địa chỉ của nó.

### Giao thức truyền dữ liệu

![](https://khuenguyencreator.com/wp-content/uploads/2020/07/H4-2.jpg "Nguồn ảnh: khuenguyencreator.com")

Nguồn ảnh: khuenguyencreator.com

Start Condition: Khi master gửi yêu cầu truyền dữ liệu đến một slave, đường truyền SDA sẽ chuyển từ mức điện áp cao xuống mức điện áp thấp trước khi SCL làm điều tương tự.

Address Frame: Địa chỉ 7 bit của slave.

R/W Bit: Dùng để xác định hướng truyền dữ liệu.
- 0: master gửi, slave nhận.
- 1: slave gửi, master nhận.

ACK/NACK Bit thứ nhất: Viết tắt của Acknowledged/No Acknowledged, là bit xác nhận.
- Nếu có địa chỉ nào trùng với địa chỉ mà master chỉ định, bit này được đặt thành 0 (ACK).
- Ngược lại, bit này nhận giá trị 1 (NACK).

Data Frame: Là dữ liệu cần truyền, gồm 8 bit và được thiết lập bởi bên gửi.

ACK/NACK Bit thứ hai
- Bên nhận nhận dữ liệu thành công, bit này nhận giá trị 0.
- Ngược lại, 1.

Stop Condition: Ngược lại với Start Condition, kể cả thứ tự của SDA và SCL.

### Các chế độ hoạt động của I2C

Dựa theo tốc độ, 2 loại:

- Chế độ chuẩn (Standard mode), tốc độ 100 Kbit/s.
- Chế độ tốc độ thấp (Low-speed mode), tốc độ 10 Kbit/s.

Theo quan hệ master/slave, 3 loại:

- 1 master, 1 slave.
- 1 master, n slave.
- n master, n slave.

## 15. Trình bày về chuẩn giao tiếp 1-wire

1-wire là chuẩn giao tiếp được thiết kế bởi Dallas Semiconductor.

Là chuẩn giao tiếp không đồng bộ và chỉ dùng một đường truyền nên có tốc độ thấp.

1-wire tuân theo mô hình master/slave nhưng chỉ cho phép 1 master. Mỗi slave có một địa chỉ 64 bit.

### Cơ chế hoạt động

Sử dụng khái niệm time-slot (khe thời gian). Mỗi time-slot là một khoảng thời gian mà trong đó mức logic 0/1 sẽ được ghi hoặc đọc.

Time-slot có khoảng thời gian là:

- 60 microseconds trong chế độ chuẩn.
- 8 microseconds trong chế độ overdrive.

![](https://nguyentiensk.files.wordpress.com/2016/09/waveforms.gif?w=614&h=376)

4 thao tác cơ bản:

- Gửi bit 0
- Gửi bit 1
- Đọc bit
- Reset

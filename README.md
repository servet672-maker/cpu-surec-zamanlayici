# CPU Süreç Zamanlayıcı Simülasyonu

Algoritma ve Programlama dersi kapsamında C dilinde hazırlanmış
CPU zamanlayıcı simülasyonu.

## Öğrenci bilgileri

- Adı Soyadı: Servet CANLI
- Öğrenci Numarası: 4260505007
- Çalışma: Bireysel proje

## Programın amacı

Program, kullanıcıdan alınan aynı süreç listesini üç farklı
zamanlama algoritmasıyla çalıştırır:

- FCFS: Geliş zamanı sırasına göre kesintisiz çalışma.
- SJF: Hazır süreçler arasından en kısa işi seçerek kesintisiz çalışma.
- Round Robin: Her turda P1’den Pn’ye dolaşarak zaman dilimiyle çalışma.

Round Robin uygulaması, ödev yönergesindeki tur tabanlı kurala uyar.

Her algoritma için Gantt şeması, süreçlerin bitiş, dönüş ve
bekleme süreleri, ortalamalar ve bağlam değişimi sayısı gösterilir.

## Dosyalar

- main.c: C kaynak kodu.
- ornek_girdi.txt: Örnek çalıştırma verileri.
- ornek_cikti.txt: Örnek verilerle elde edilen çıktı.

## Dev-C++ ile çalıştırma

1. main.c dosyasını indirin.
2. Dev-C++ içinde Dosya > Aç menüsünden açın.
3. Derle ve Çalıştır seçeneğini kullanın.
4. Süreç sayısını, geliş ve CPU sürelerini, ardından zaman dilimini girin.

## GCC ile çalıştırma

Derleme:

```sh
gcc -std=c99 -Wall -Wextra -pedantic main.c -o cpu_sim
```

Linux veya macOS üzerinde örnek girdiyi kullanma:

```sh
./cpu_sim < ornek_girdi.txt
```

Windows Komut İstemi üzerinde örnek girdiyi kullanma:

```bat
cpu_sim.exe < ornek_girdi.txt
```

## Örnek girdi

```text
3
0 5
0 3
0 1
2
```

İlk sayı süreç sayısıdır. Sonraki üç satırda geliş zamanı ve
CPU süresi bulunur. Son sayı Round Robin zaman dilimidir.

## Örnek sonuç

| Algoritma | Ortalama bekleme | Ortalama dönüş | Bağlam değişimi |
|---|---:|---:|---:|
| FCFS | 4,33 | 7,33 | 2 |
| SJF | 1,67 | 4,67 | 2 |
| Round Robin | 4,33 | 7,33 | 5 |

Bu örnekte en düşük ortalama bekleme süresini SJF sağlar.

## Uygulama özellikleri

- Yalnızca stdio.h kullanılır.
- struct, malloc ve free kullanılmaz.
- Süreç bilgileri paralel dizilerde tutulur.
- Ortalamalar pointer parametrelerle aktarılır.
- Gantt satır sayacı static yerel değişkendir.
- Her algoritmadan önce çalışma verileri sıfırlanır.
- Gantt şemasında bir satırda en fazla 6 blok gösterilir.

## Girdi sınırları

- Süreç sayısı: 1–100
- Geliş zamanı: 0–10000
- CPU süresi: 1–10000
- Zaman dilimi: 1–10000

Geçersiz girişte program hata mesajı vererek sonlanır.
Bağlam değişimleri için ek zaman maliyeti hesaplanmaz.

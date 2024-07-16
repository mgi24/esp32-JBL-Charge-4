# esp32-JBL-Charge-4
an ESP 32 arduino code for replacing JBL Charge 4 Qualcomm CSR 8675 Module

Cara Pakai:

1. Download Github ini
   ![image](https://github.com/user-attachments/assets/73e1fa84-0c32-4024-8b5a-9a66bb5a76ed)
2. Extract filenya dan buka file volumebutton.ino memakai arduino IDE
3. Pastikan startup.h, maxvol.h, dan shutdown.h semuanya tampil di Arduino IDE
   ![image](https://github.com/user-attachments/assets/66102db8-dbdf-49eb-96ee-e621d068f47a)
   buat folder baru bernama volumebutton dan tarik semua filenya kesana misal gak tampil
   
4. Download 3 library ini, caranya download seperti langkah no1
   https://github.com/pschatzmann/ESP32-A2DP
   https://github.com/pschatzmann/arduino-audio-tools
   https://github.com/earlephilhower/ESP8266Audio
   
5. Klik sketch > Include Library > Add .ZIP Library kemudian pilih 3 library yang sudah didownload di langkah no4, install satu persatu
   ![image](https://github.com/user-attachments/assets/3e0fa54a-a229-42f5-b1a1-8c0036ca8c08)

6. Klik tools > board > board manager...
   ![image](https://github.com/user-attachments/assets/28fef5a8-217e-4e3f-b956-d058bad607bf)
7. Cari "ESP32" dan install versi agak lawas bila tidak berhasil diflash, saya menggunakan versi 2.0.12 contohnya
8. Coba Verify dengan klik centang, harusnya gada error sih
9. Pilih port arduino yang sesuai dan pastikan board ESP32 yang anda pilih benar!
    ![image](https://github.com/user-attachments/assets/1f4b94af-b8a8-4305-a3b1-97dc30821ea3)
10. Ubah FLASH SIZE dan PARTITION SCHEME di tools menjadi 4MB dan HUGE APP
    ![image](https://github.com/user-attachments/assets/df59b4c8-cbd0-4790-bb02-3ed2edafbda0)
11. Silahkan coba flash

Ubah Kode:
Silahkan bebas ubah kode sesuka kalian, kalian bisa ubah suara on off di startup dan shutdown sesuka kalian.

Untuk vol boost apabila kalian pakai PCM5102 tidak bisa ya! jadi gak guna!

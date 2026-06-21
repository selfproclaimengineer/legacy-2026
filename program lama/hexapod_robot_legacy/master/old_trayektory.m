rate = 5;
rate1 = (rate + 1);
rate2 = ((rate * 2) + 1);
rate3 = ((rate * 3) + 1);
rate4 = ((rate * 4) + 1);
coxa = 77;  %30
femur = 49;
posisiAwal = ((coxa + femur) / 1);  %1.5

%-----------------ubah disini-----------------
Zoff = -45;
posisiX = posisiAwal;
%posisiY = posisiAwal;
posisiY = 0;
posisiZ = Zoff - 20;
lebarLangkah = 60;  %lebar langkah dibagi rate*2 harus bulat 40 4 32 //harus bulat agar stabil
tinggiLangkah = 80;
%----------------------------------------------

jumlahPosisiKaki = ((rate * 2) + 1);
posisi = jumlahPosisiKaki;
gerakan = 1;
langkahDatar = lebarLangkah / (rate * 2);
langkahNaik = (tinggiLangkah / rate);
x = zeros(1, 50);
y = zeros(1, 50);
z = zeros(1, 50);

for i = 1: 1: 50
    if gerakan == 0  %gerak segitiga
       if posisi < rate1  %(rate+1))
         posisiX = posisiX +  langkahDatar;
         posisiY = posisiY + langkahDatar;
         posisiZ = posisiZ + langkahNaik;
         posisi = posisi + 1;
       elseif posisi >= rate1 && posisi < rate2  %leg[idLeg].posisi>=(rate+1) && leg[idLeg].posisi<((rate*2)+1)
         posisiX = posisiX +  langkahDatar;
         posisiY = posisiY + langkahDatar;
         posisiZ = posisiZ - langkahNaik;
         posisi = posisi + 1;
       end
       if posisi == rate2 
         gerakan = 1;
       end
    elseif gerakan == 1 %dorong
       if posisi > 1 
         posisiX = posisiX -  langkahDatar;
         posisiY = posisiY - langkahDatar;
         posisi = posisi - 1;
       end
       if posisi == 1 
         gerakan = 0;
       end
    end
    y(i,:) = posisiY;
    x(i,:) = posisiX;
    z(i,:) = posisiZ;
end
plot3(x, y, z);
grid
xlabel('x')
ylabel('y')
zlabel('z')
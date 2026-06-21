

void followUtara(int toleransi, int step, int lebar) {
    knop = 0;
    toler = toleransi;
    lebarLangkah = 40;
    sudut = bacaKompas();

    while (!((sudut >= utara - toleransi) && (sudut <= utara + toleransi))) {
        lebarLangkah = lebar;
        sudut = bacaKompas();
        
        if (sudut < (utara - toleransi) && sudut >= 0) {
            for (int i = 0; i < step; i++) {
                passData(6);  // Pivot kanan
            }
        } else if (sudut > selatan && sudut <= 199) {
            for (int i = 0; i < step; i++) {
                passData(6);  // Pivot kanan
            }
        } else if (sudut > (utara + toleransi) && sudut <= selatan) {
            for (int i = 0; i < step; i++) {
                passData(5);  // Pivot kiri
            }
        }
    }
}

void followSelatan(int toleransi, int step, int lebar) {
    knop = 2;
    toler = toleransi;
    lebarLangkah = 40;
    sudut = bacaKompas();

    while (!((sudut >= (selatan - toleransi)) && (sudut <= (selatan + toleransi)))) {
        lebarLangkah = lebar;
        sudut = bacaKompas();
        
        if (sudut >= (selatan - toleransi) && sudut <= 199) {
            for (int i = 0; i < step; i++) {
                passData(5);  // Pivot kiri
            }
        } else if (sudut < utara && sudut >= 0) {
            for (int i = 0; i < step; i++) {
                passData(5);  // Pivot kiri
            }
        } else if (sudut < (selatan - toleransi) && sudut > utara) {
            for (int i = 0; i < step; i++) {
                passData(6);  // Pivot kanan
            }
        }
    }
}

void followBarat(int toleransi, int step, int lebar) {
    knop = 3;
    toler = toleransi;
    lebarLangkah = 40;
    sudut = bacaKompas();

    while (!((sudut >= barat - toleransi) && (sudut <= barat + toleransi))) {
        lebarLangkah = lebar;
        sudut = bacaKompas();
        
        if (sudut >= (barat - toleransi) && sudut <= 199) {
            for (int i = 0; i < step; i++) {
                passData(5);  // Pivot kiri
            }
        } else if (sudut < timur && sudut >= 0) {
            for (int i = 0; i < step; i++) {
                passData(5);  // Pivot kiri
            }
        } else if (sudut <= (barat + toleransi) && sudut >= timur) {
            for (int i = 0; i < step; i++) {
                passData(6);  // Pivot kanan
            }
        }
    }
}

void followTimur(int toleransi, int step, int lebar) {
    knop = 1;
    toler = toleransi;
    lebarLangkah = 40;
    sudut = bacaKompas();

    while (!((sudut >= timur - toleransi) && (sudut <= timur + toleransi))) {
        lebarLangkah = lebar;
        sudut = bacaKompas();
        
        if (sudut >= barat && sudut <= 199) {
            for (int i = 0; i < step; i++) {
                passData(6);  // Pivot kanan
            }
        } else if (sudut < (timur + toleransi) && sudut >= 0) {
            for (int i = 0; i < step; i++) {
                passData(6);  // Pivot kanan
            }
        } else if (sudut >= (timur - toleransi) && sudut <= barat) {
            for (int i = 0; i < step; i++) {
                passData(5);  // Pivot kiri
            }
        }
    }
}

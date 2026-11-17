#include <stdio.h>

#define MAX_N 100
#define MAX_DEGER 10000
#define SATIR_BLOK 6

/* Yazan: [Servet CANLI] - Sinirli pozitif veya sifir tamsayi okur.
   Cok uzun sayilari da tasma olusturmadan reddeder. */
int sayi_oku(int alt, int ust, int *sonuc)
{
    int c, sayi = 0, rakam = 0, hata = 0;
    do { c = getchar(); }
    while (c == ' ' || c == '\n' || c == '\t' || c == '\r');
    if (c == EOF) return 0;
    while (c != EOF && c != ' ' && c != '\n' && c != '\t' && c != '\r') {
        if (c < '0' || c > '9') hata = 1;
        else {
            rakam = 1;
            if (!hata) {
                if (sayi > (ust - (c - '0')) / 10) hata = 1;
                else sayi = sayi * 10 + c - '0';
            }
        }
        c = getchar();
    }
    if (hata || !rakam || sayi < alt || sayi > ust) return 0;
    *sonuc = sayi;
    return 1;
}

/* Yazan: [Servet CANLI] - Gantt blogunu yazar, son sureci ve gecisleri izler.
   id=-1 cagrisinda static yerel satir sayaci sifirlanir. id=0 bostadir. */
void gantt(int id, long bas, long son, int *onceki, int *gecis)
{
    static int satirdaki = 0;
    if (id == -1) { satirdaki = 0; return; }
    if (id == 0) printf("[BOS %ld-%ld] ", bas, son);
    else {
        printf("[P%d %ld-%ld] ", id, bas, son);
        if (*onceki != 0 && *onceki != id) (*gecis)++;
        *onceki = id;
    }
    satirdaki++;
    if (satirdaki == SATIR_BLOK) { printf("\n"); satirdaki = 0; }
}

/* Yazan: [Servet CANLI] - Her algoritma oncesi calisma verilerini sifirlar. */
void sifirla(int n, const int cpu[], int kalan[], long bitis[],
             long *saat, int *onceki, int *gecis)
{
    int i;
    for (i = 0; i < n; i++) { kalan[i] = cpu[i]; bitis[i] = 0; }
    *saat = 0; *onceki = 0; *gecis = 0;
    gantt(-1, 0, 0, onceki, gecis);
}

/* Yazan: [Servet CANLI] - Bitmemis sureclerin en erken gelisini bulur. */
long sonraki_gelis(int n, const int gelis[], const int kalan[])
{
    int i;
    long en_erken = -1;
    for (i = 0; i < n; i++)
        if (kalan[i] > 0 && (en_erken == -1 || gelis[i] < en_erken))
            en_erken = gelis[i];
    return en_erken;
}

/* Yazan: [Servet CANLI] - FCFS: gelis ve surec numarasina gore calistirir. */
void fcfs(int n, const int gelis[], const int cpu[], int kalan[],
          long bitis[], long *saat, int *onceki, int *gecis)
{
    int i, sec, tamam;
    for (tamam = 0; tamam < n; tamam++) {
        sec = -1;
        for (i = 0; i < n; i++)
            if (kalan[i] > 0 && (sec == -1 || gelis[i] < gelis[sec])) sec = i;
        /* Tarama kucuk indeksten baslar; esitlikte sec degismez. */
        if (*saat < gelis[sec]) {
            gantt(0, *saat, gelis[sec], onceki, gecis);
            *saat = gelis[sec];
        }
        gantt(sec + 1, *saat, *saat + cpu[sec], onceki, gecis);
        *saat += cpu[sec]; bitis[sec] = *saat; kalan[sec] = 0;
    }
}

/* Yazan: [Servet CANLI] - Kesintisiz SJF: hazir sureclerden en kisayi secer. */
void sjf(int n, const int gelis[], const int cpu[], int kalan[],
         long bitis[], long *saat, int *onceki, int *gecis)
{
    int i, sec, tamam = 0;
    long yeni;
    while (tamam < n) {
        sec = -1;
        for (i = 0; i < n; i++) {
            if (kalan[i] > 0 && gelis[i] <= *saat) {
                if (sec == -1 || cpu[i] < cpu[sec] ||
                    (cpu[i] == cpu[sec] && gelis[i] < gelis[sec])) sec = i;
            }
        }
        if (sec == -1) {
            yeni = sonraki_gelis(n, gelis, kalan);
            gantt(0, *saat, yeni, onceki, gecis); *saat = yeni;
        } else {
            gantt(sec + 1, *saat, *saat + cpu[sec], onceki, gecis);
            *saat += cpu[sec]; bitis[sec] = *saat;
            kalan[sec] = 0; tamam++;
        }
    }
}

/* Yazan: [Servet CANLI] - Yonergedeki P1..Pn tur tabanli Round Robin. */
void round_robin(int n, const int gelis[], int kalan[], long bitis[],
                 int quantum, long *saat, int *onceki, int *gecis)
{
    int i, sure, calisti, tamam = 0;
    long yeni;
    while (tamam < n) {
        calisti = 0;
        for (i = 0; i < n; i++) {
            if (kalan[i] > 0 && gelis[i] <= *saat) {
                sure = kalan[i] < quantum ? kalan[i] : quantum;
                gantt(i + 1, *saat, *saat + sure, onceki, gecis);
                *saat += sure; kalan[i] -= sure; calisti = 1;
                if (kalan[i] == 0) { bitis[i] = *saat; tamam++; }
            }
        }
        if (!calisti && tamam < n) {
            yeni = sonraki_gelis(n, gelis, kalan);
            gantt(0, *saat, yeni, onceki, gecis); *saat = yeni;
        }
    }
}

/* Yazan: [Serve CANLI] - Iki ortalamayi pointer parametrelerle dondurur. */
void ortalamalar(int n, const int gelis[], const int cpu[],
                 const long bitis[], double *bek, double *don)
{
    int i;
    double toplam_bek = 0, toplam_don = 0;
    for (i = 0; i < n; i++) {
        toplam_don += bitis[i] - gelis[i];
        toplam_bek += bitis[i] - gelis[i] - cpu[i];
    }
    *bek = toplam_bek / n; *don = toplam_don / n;
}

/* Yazan: [Servet CANLI] - Surec numarasi sirasiyla sonuc tablosunu yazar. */
void tablo(int n, const int gelis[], const int cpu[], const long bitis[],
           double bek, double don, int gecis)
{
    int i;
    printf("\nSurec  Gelis  CPU    Bitis    Donus    Bekleme\n");
    for (i = 0; i < n; i++)
        printf("P%-5d %-6d %-6d %-8ld %-8ld %ld\n", i + 1, gelis[i], cpu[i],
               bitis[i], bitis[i] - gelis[i], bitis[i] - gelis[i] - cpu[i]);
    printf("Ortalama bekleme: %.2f\nOrtalama donus: %.2f\nBaglam degisimi: %d\n",
           bek, don, gecis);
}

/* Yazan: [Servet CANLI] - Girdiyi bir kez okur; uc algoritmayi karsilastirir. */
int main(void)
{
    int n, i, quantum, onceki, gecis, en_iyi = 0;
    int gelis[MAX_N], cpu[MAX_N], kalan[MAX_N];
    long bitis[MAX_N], saat;
    double bek[3], don[3];
    const char *ad[3] = {"FCFS", "SJF", "Round Robin"};

    printf("Surec sayisi (1-%d): ", MAX_N);
    if (!sayi_oku(1, MAX_N, &n)) { printf("Gecersiz surec sayisi.\n"); return 1; }
    for (i = 0; i < n; i++) {
        printf("P%d gelis (0-%d) ve CPU (1-%d): ", i + 1, MAX_DEGER, MAX_DEGER);
        if (!sayi_oku(0, MAX_DEGER, &gelis[i]) || !sayi_oku(1, MAX_DEGER, &cpu[i])) {
            printf("Gecersiz surec verisi.\n"); return 1;
        }
    }
    printf("Zaman dilimi (1-%d): ", MAX_DEGER);
    if (!sayi_oku(1, MAX_DEGER, &quantum)) { printf("Gecersiz zaman dilimi.\n"); return 1; }

    for (i = 0; i < 3; i++) {
        sifirla(n, cpu, kalan, bitis, &saat, &onceki, &gecis);
        printf("\n=== %s ===\n", ad[i]);
        if (i == 0) fcfs(n, gelis, cpu, kalan, bitis, &saat, &onceki, &gecis);
        else if (i == 1) sjf(n, gelis, cpu, kalan, bitis, &saat, &onceki, &gecis);
        else round_robin(n, gelis, kalan, bitis, quantum, &saat, &onceki, &gecis);
        ortalamalar(n, gelis, cpu, bitis, &bek[i], &don[i]);
        tablo(n, gelis, cpu, bitis, bek[i], don[i], gecis);
    }
    for (i = 1; i < 3; i++) if (bek[i] < bek[en_iyi]) en_iyi = i;
    printf("\nKARSILASTIRMA\nFCFS: %.2f | SJF: %.2f | Round Robin: %.2f\n",
           bek[0], bek[1], bek[2]);
    printf("En dusuk ortalama bekleme: %s\n", ad[en_iyi]);
    return 0;
}

# Sürü Drone Simülasyonu (Swarm Drone Flight Simulation)

C++ ile geliştirilmiş, **Raylib 5.0** tabanlı gerçek zamanlı, görsel sürü drone uçuş simülasyonu. Farklı formasyon tipleri, taktiksel uçuş modelleri ve davranışsal uçuş senaryolarını terminal menüsü üzerinden seçerek anlık olarak bir GUI üzerinde simüle eder.

## Derleme ve Çalıştırma (Windows - MinGW)

Projede `Raylib 5.0` kullanıldığından dolayı sisteme minGW DLL'lerinin doğru şekilde tanıtılması gerekmektedir. Projeyi yeniden derlemek ve anında çalıştırmak için aşağıdaki bat dosyalarını kullanabilirsiniz:

### 1. Yeniden Derlemek İçin:
```cmd
build.bat
```
*(Bu komut `src` içindeki tüm C++ dosyalarını toplayıp `SwarmDroneSim.exe` adıyla statik olarak derler ve logları `build.log`a kaydeder.)*

### 2. Simülasyonu Çalıştırmak İçin:
```cmd
run.bat
```
*(Bu komut MinGW kütüphanelerini geçici PATH'e ekleyerek `.exe` dosyasını sorunsuzca başlatır. Ekrana menü gelir.)*

> **Not:** Sisteminizde standart bir MSYS2/MinGW kurulumu (`C:\msys64\mingw64\bin`) bulunduğu varsayılarak tasarlanmıştır.

## Proje Yapısı

```
include/
├── Vec3.h          # 3D vektör matematiği
├── Drone.h         # Model: Tekil Drone özellikleri (hız, hedef vb.)
├── Swarm.h         # Controller: Sürü yöneticisi
├── Simulation.h    # Arayüz: Simülasyon temel abstract sınıfı
├── Visualizer.h    # View: Raylib GUI Görselleştirici altyapısı
└── simulations/    # Formasyon ve taktiksel senaryolar

src/
├── main.cpp        # Giriş Noktası & Menü & Raylib Loop
├── Drone.cpp       # Drone hareket hesaplamaları
├── Swarm.cpp       # Sürü fonksiyonları
├── Visualizer.cpp  # GUI Drone çizimleri ve Grid rendering
└── simulations/    # Simülasyon senaryo implementasyonları
```

## Simülasyon Listesi

### Geometrik Formasyonlar
- [ ] V Formasyonu
- [ ] Elmas (Diamond) Formasyonu
- [ ] Grid (Izgara) Formasyonu
- [ ] Çizgi (Line Abreast) Formasyonu

### Taktiksel Uçuş
- [ ] Sızma (Infiltration)
- [ ] Doyurma Saldırısı (Saturation Attack)
- [ ] Sanal Yapı (Virtual Structure)

### Davranışsal Modeller
- [ ] Takip ve Kuşatma (Surround & Pursue)
- [ ] Kendi Kendini İyileştirme (Self-Healing)
- [ ] Bölgesel Devriye (Swarm CAP)

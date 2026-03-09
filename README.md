# Sürü Drone Simülasyonu (Swarm Drone Flight Simulation)

C++ ile geliştirilmiş sürü drone uçuş simülasyonu. Farklı formasyon tipleri, taktiksel uçuş modelleri ve davranışsal uçuş senaryolarını simüle eder.

## Derleme (Build)

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Çalıştırma

```bash
./bin/SwarmDroneSim     # Linux/Mac
.\bin\Debug\SwarmDroneSim.exe  # Windows
```

## Proje Yapısı

```
include/
├── Vec3.h          # 3D vektör matematiği
├── Drone.h         # Drone modeli
├── Swarm.h         # Sürü yöneticisi
├── Simulation.h    # Simülasyon arayüzü (abstract)
└── simulations/    # Simülasyon implementasyonları (ileriki aşamalar)
src/
├── main.cpp        # Giriş noktası + menü
├── Drone.cpp       # Drone implementasyonu
├── Swarm.cpp       # Swarm implementasyonu
└── simulations/    # Simülasyon implementasyonları (ileriki aşamalar)
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

# Fractal Image Generation using TBB

## Initial Code Profiling

Running times (in milliseconds) on 5000×3000 images for 1000 iterations:

- _Mandelbrot:_ 12 753 ms
- _Julia:_ 3 961 ms
- _Tricorn:_ 3 860 ms
- _Cosine:_ 308 981 ms

---

### Mandelbrot

#### CPU Utilisation

![Mandelbrot CPU Utilisation](img/profile/profile-serial-mandelbrot-cpu.png)

#### Top Hotspots

![Mandelbrot Top Hotspots](img/profile/profile-serial-mandelbrot-top.png)

---

### Julia

#### CPU Utilisation

![Julia CPU Utilisation](img/profile/profile-serial-julia-cpu.png)

#### Top Hotspots

![Julia Top Hotspots](img/profile/profile-serial-julia-top.png)

---

### Tricorn

#### CPU Utilisation

![Tricorn CPU Utilisation](img/profile/profile-serial-tricorn-cpu.png)

#### Top Hotspots

![Tricorn Top Hotspots](img/profile/profile-serial-tricorn-top.png)

---

### Cosine

#### CPU Utilisation

![Cosine CPU Utilisation](img/profile/profile-serial-cosine-cpu.png)

#### Top Hotspots

![Cosine Top Hotspots](img/profile/profile-serial-cosine-top.png)

### Small optimisations

After analysing the above hotspots, I made the following small optimisations:

- I used a constant for the escape radius instread of getting it from a virtual function.
- I used the `std::abs` function instead of the `std::norm` function to check the magnitude of a complex number. It seems that the `std::norm` function is slower than the `std::abs` function.

Running times (in milliseconds) on 5000×3000 images for 1000 iterations:

- _Mandelbrot:_ 6 900 ms
- _Julia:_ 2 148 ms
- _Tricorn:_ 2 157 ms
- _Cosine:_ 301 380 ms

## Initial TBB Implementation

Running times (in milliseconds) on 5000×3000 images for 1000 iterations:

- _Mandelbrot:_ 849 ms
- _Julia:_ 253 ms
- _Tricorn:_ 273 ms
- _Cosine:_ 46 451 ms

### Mandelbrot

#### CPU Utilisation

![Mandelbrot CPU Utilisation](img/profile/profile-tbb-initial-mandelbrot-cpu.png)

#### Worker Utilisation

![Mandelbrot Worker Utilisation](img/profile/profile-tbb-initial-mandelbrot-work.png)

---

### Julia

#### CPU Utilisation

![Julia CPU Utilisation](img/profile/profile-tbb-initial-julia-cpu.png)

#### Worker Utilisation

![Julia Worker Utilisation](img/profile/profile-tbb-initial-julia-work.png)

---

### Tricorn

#### CPU Utilisation

![Tricorn CPU Utilisation](img/profile/profile-tbb-initial-tricorn-cpu.png)

#### Worker Utilisation

![Tricorn Worker Utilisation](img/profile/profile-tbb-initial-tricorn-work.png)

---

### Cosine

#### CPU Utilisation

![Cosine CPU Utilisation](img/profile/profile-tbb-initial-cosine-cpu.png)

#### Worker Utilisation

![Cosine Worker Utilisation](img/profile/profile-tbb-initial-cosine-work.png)

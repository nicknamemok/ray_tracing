Modified version of code from "Ray Tracing in One Weekend" (https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- Changes to class structures -> includes a scene class that makes it easier to multithread.
- Removed hit_record structure that was used to pass information around. Hit information is now innate to each object per ray.
- Now writes to file which slightly speeds up rendering.

# CSC4140 Assignment2

Haopeng Chen 120090645

## How to Start

**Store an Image with 60 Degrees Rotation**

```bash
sh ./scratch.sh -r 60
```

**Store an Image named *abc.png* with 60 Degrees Rotation**

```bash
sh ./scratch.sh -r 60 abc.png
```

**Have a Free Windowed View**

```bash
sh ./scratch.sh
```

***Note:** Make sure you have dependencies. Use `A` and `D` to control angle,`Esc` for quit.*

**Custom:** In `main`, change `Eigen::Vector3f T` for translation, `Eigen::Vector3f S` for scale, `Eigen::Vector3f P0`
and `Eigen::Vector3f P1` for rotation axis.
# Simple PLY ASCII file reader 

This is a code to read PLY files in ASCII format. The PLY files are a geometrical representation of a mesh, and it was developed for the [Stanford University](http://graphics.stanford.edu/data/3Dscanrep/). Code is using the [RPly library](http://w3.impa.br/~diego/software/rply/), developed by Diego Nehab. In this version, is more **readable** than original examples to be used easily.

The project creates a namespace called `Surface`. A `Surface` contains a `std::vector` of `vert` and `face`, defined as:
```cpp
struct vert
{
    float x;
	float y;
	float z;
};
struct face
{
	int id0;
	int id1;
	int id2;
};
```

Both vertexes and faces, are read using callback functions (it is possible to use lambda functions actually). You can use the namespace as:
```cpp
if (Surface::loadFile(filename)){
    // your code here
}
```
In `main.cpp` there is a snippet code for that, using the `surfaceAB.ply` file.

###### If you want to contribute to this project and make it better, your help is very welcome.

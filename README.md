# Model Loader
An OpenGL 3D object loader, supporting .obj and .dae files.

## Packages, Dependencies and Version
OpenGL Version 4.6.0  
GLEW Version 1.2.3.4  
Visual Studio 2019 16.3.6  

To install the required dependencies and packages within Visual Studio, right click on the solution and press 'Restore Nuget Packages'. Once complete, right click on the solution once more and press 'Rescan'.

## Instructions For Use
- Upon initial boot you will be prompted to enter the relative path of the model(s) you wish to load. Once entered press the enter key to start rendering.*These must be type .obj or .dae.*
- To render multiple objects within the scene one must enter each relative file path separated by a white space.  
( i.e. -> *models/creeper/creeper.obj  models/low_poly_boat/low_poly_boat.dae* )
- If a model fails to load the terminal will inform you of such.
- Please be aware that large models may take a considerable period of time to load and multiple instances of the same model will load on top of one another and will need to be scaled and/or translated to become visible.
- To load a new set of files the application must be restarted.

## Controls
TAB ~ Toggle currently selected model.  
BACKSPACE ~ Delete currently selected model.  
ESC ~ Close program.

#### Model Movement
W ~ Move model up  
A ~ Move model left  
S ~ Move model down  
D ~ Move model right

#### Model Rotations  
Up Arrow ~ Rotate forward  
Left Arrow ~ Rotate left  
Down Arrow ~ Rotate backwards  
Left Arrow ~ Rotate right  

#### Scale
NUM PAD + ~ Scale up  
NUM PAD - ~ Scale down

## Architecture
The main principles of the architecture of the software are as follows:

- The main entry point of the program prompts the user to input the file path of the model they wish to load.
- The file paths are extracted from the string, dependant on the extension of the file, the path string is handed to either a **DaeReader** or **ObjReader** object which returns a pointer to a **Model**.
- This Model is then added to a vector of Models within the program who's **Draw** methods are called every game loop.
 - A Model is compose of a vector of type **Object**, which is composed of a vector of type **Mesh**.
 - When the Draw method is called on a Model, the Draw method is called for each composing **Object**, and in turn the Draw method on each **Mesh**.
 - The final Draw call on the cascaded **Mesh** binds the Vertex Array and applies any textures.
- In a similar vein to creating a model. Calling the Delete method on a model will cascade down to the composing Meshes and call its respective Delete method.


#### Readers
- Parse file extracting Vertex data: normals, vertices, colours, indices, materials and construct a model.

  **OBJ**
  - Construct a new Object upon reaching a new object identifier 'o'.
  - Construct a new Mesh upon reaching a use material identifier 'usemtl'.
  - All meshes are added to their respective object, with objects then being added to the model.

  **DAE**
  - Construct a single Mesh and add it to a single object, constructing a model.


#### Model  
- Holds a vector of objects that compose a model
- Hold the models position, scale and rotational data and relevant methods for their manipulation.

#### Object
- Hold a vector of meshes that compose an object

#### Mesh
- Contain a vector of Vertex structs.
- Hold a material object.
- Initialise and Bind Vertex and material data to relative VBO and VAO.
- Pass data to shader.

#### Material
- A Class containing material information extracted from file.

#### Vertex
- A Struct that holds vertices' respective texture, colour, position and normal co-ordinates.

## Evaluations and Considerations
Although the solution provided is successful in reading in the provided models. There are some clear improvements that could be made to the code base to increase efficiency, speed and extendibility.  
They are as follows:
- Implement Reader class inheritance from a class that defines share functionality required across all readers.
- Implement a Utility class that can be instantiated to perform common tasks such as string and file manipulations/checks.
- Implement more data reference passing, reducing the number of variable instantiation and memory footprint.

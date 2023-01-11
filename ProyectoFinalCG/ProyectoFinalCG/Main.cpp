/*
Semestre 2023-1
Animación:
1.- Simple o básica:Por banderas y condicionales
2.- Compleja: Por medio de funciones y algoritmos,Textura Animada.
4.- Técnicas de Animación: Por Keyframes
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>r
#include <vector>
#include <math.h>
#include <windows.h>
#include <playsoundapi.h>
#include <Mmsystem.h>
#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movCoche1;
float movHel;
float movHel1;
float movCo1;
float movCo2;
float movCo3;
float movCo4;
float movOffset;
float rotllanta;
float rotllanta1;
float rotllanta2;
float rotllantaOffset;
bool avanza;
bool avanza1;
bool avanza2;
bool avanzac2;
bool avanzac1;
bool avanzac3;
bool avanzac4;
float toffsetu = 0.0f;
float toffsetv = 0.0f;

const float PI = 3.14159265f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;

Texture FlechaTexture;
Texture FlechaTexture1;

Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;
Model Dado_M;
Model Pista_M;
Model Me_M;
Model PuestoTortas1;
Model PuestoTacos1;
Model Fdhd;
Model Fdpi;
Model Dongato;
Model Poste_M;
Model Sushi;
Model Aguas;
Model Verduras;
Model Brochetas;
Model Teatro;
Model Tortas;
Model Trompo;
Model Plato;
Model Lpizza;
Model Globo;
Model Bote;
Model Postedg;
Model Pareddg;
Model Pajaro;

Skybox skyboxDay;
Skybox skyboxNight;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLightDay;
DirectionalLight mainLightNight;

//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
PointLight staticPL[MAX_POINT_LIGHTS];
SpotLight staticSL[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};


	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CreateToroid(int res) {

	//constantes utilizadas en los ciclos for
	int n, i, coordenada = 0;
	//número de vértices ocupados
	int verticesBase = (res + 1) * 48;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = 0.0f;
	//apuntadores para guardar todos los vértices e índices generados
	GLfloat* vertices = (GLfloat*)calloc(sizeof(GLfloat*), (verticesBase));
	unsigned int* indices = (unsigned int*)calloc(sizeof(unsigned int*), verticesBase / 8);

	for (n = 0; n <= res; n++)
	{
		x = cos((n)*dt);
		z = sin((n)*dt);

		printf("%f | %f \n", x, z);

		//Generamos el vertice interior
		for (i = 0; i < 8; i++) {
			switch (i)
			{
			case 0:
				vertices[i + coordenada] = x;
				break;
			case 1:
				vertices[i + coordenada] = y;
				break;
			case 2:
				vertices[i + coordenada] = z;
				break;
			default:
				vertices[i + coordenada] = 0.0f;
				break;
			}
		}
		coordenada += 8;

		//Generamos el vertice exterior
		for (i = 0; i < 8; i++) {
			switch (i)
			{
			case 0:
				vertices[i + coordenada] = x * 2;
				break;
			case 1:
				vertices[i + coordenada] = y;
				break;
			case 2:
				vertices[i + coordenada] = z * 2;
				break;
			default:
				vertices[i + coordenada] = 0.0f;
				break;
			}
		}
		coordenada += 8;

		//Generamos el ápice
		for (i = 0; i < 8; i++) {
			switch (i)
			{
			case 0:
				vertices[i + coordenada] = x;
				break;
			case 1:
				vertices[i + coordenada] = y + 1;
				break;
			case 2:
				vertices[i + coordenada] = z;
				break;
			default:
				vertices[i + coordenada] = 0.0f;
				break;
			}
		}
		coordenada += 8;

		//Generamos el vertice interior
		for (i = 0; i < 8; i++) {
			switch (i)
			{
			case 0:
				vertices[i + coordenada] = x;
				break;
			case 1:
				vertices[i + coordenada] = y + 1;
				break;
			case 2:
				vertices[i + coordenada] = z;
				break;
			default:
				vertices[i + coordenada] = 0.0f;
				break;
			}
		}
		coordenada += 8;

		//Generamos el vertice exterior
		for (i = 0; i < 8; i++) {
			switch (i)
			{
			case 0:
				vertices[i + coordenada] = x * 2;
				break;
			case 1:
				vertices[i + coordenada] = y + 1;
				break;
			case 2:
				vertices[i + coordenada] = z * 2;
				break;
			default:
				vertices[i + coordenada] = 0.0f;
				break;
			}
		}
		coordenada += 8;

		//Generamos el ápice
		for (i = 0; i < 8; i++) {
			switch (i)
			{
			case 0:
				vertices[i + coordenada] = x * 2;
				break;
			case 1:
				vertices[i + coordenada] = y;
				break;
			case 2:
				vertices[i + coordenada] = z * 2;
				break;
			default:
				vertices[i + coordenada] = 0.0f;
				break;
			}
		}
		coordenada += 8;

	}

	for (i = 0; i < verticesBase / 8; i++) {
		indices[i] = i;
	}

	//se genera el mesh del toroide
	Mesh* toroid = new Mesh();
	toroid->CreateMesh(vertices, indices, verticesBase, verticesBase / 8);
	meshList.push_back(toroid);

}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CreateToroid(256);

	bool dayCicleFlag = true;
	time_t initialTime = time(NULL);
	time_t currentTime = time(NULL);

	std::vector<std::string> skyboxFacesDay;
	std::vector<std::string> skyboxFacesNight;

	//Indices de luces
	int pizzasIndex = 0;
	int hotDIndex = 0;
	int tacosIndex = 0;
	int tortasIndex = 0;
	int sushiIndex = 0;
	int verdurasIndex = 0;
	int aguasIndex = 0;
	int banderillasIndex = 0;

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/pisoladrillo.tga");
	pisoTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/LetreroTacostga.tga");
	FlechaTexture.LoadTextureA();
	FlechaTexture1 = Texture("Textures/LetreroTortastga.tga");
	FlechaTexture1.LoadTextureA();

	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Camino_M = Model();
	Camino_M.LoadModel("Models/railroad track.obj");
	Pista_M = Model();
	Pista_M.LoadModel("Models/pista.obj");
	Me_M = Model();
	Me_M.LoadModel("Models/mitsev3obj.obj");
	Poste_M = Model();
	Poste_M.LoadModel("Models/posteobj.obj");
	PuestoTortas1 = Model();
	PuestoTortas1.LoadModel("Models/PuestoTortas2.obj");
	PuestoTacos1 = Model();
	PuestoTacos1.LoadModel("Models/PuestoTacos2.obj");
	Trompo = Model();
	Trompo.LoadModel("Models/Trompo.obj");
	Fdhd = Model();
	Fdhd.LoadModel("Models/Foodthd.obj");
	Fdpi = Model();
	Fdpi.LoadModel("Models/Foodtpi.obj");
	Dongato = Model();
	Dongato.LoadModel("Models/Dongato.obj");
	Aguas = Model();
	Aguas.LoadModel("Models/carroAguas.obj");
	Sushi = Model();
	Sushi.LoadModel("Models/puestoSushi.obj");
	Verduras = Model();
	Verduras.LoadModel("Models/foodtruckVerduras.obj");
	Brochetas = Model();
	Brochetas.LoadModel("Models/puestoBrochetas.obj");
	Tortas = Model();
	Tortas.LoadModel("Models/Torttas.obj");
	Teatro = Model();
	Teatro.LoadModel("Models/teatro.obj");
	Plato = Model();
	Plato.LoadModel("Models/platosushi.obj");
	Lpizza = Model();
	Lpizza.LoadModel("Models/letreropizza.obj");
	Globo = Model();
	Globo.LoadModel("Models/globo.obj");
	Bote = Model();
	Bote.LoadModel("Models/Botedg.obj");
	Postedg = Model();
	Postedg.LoadModel("Models/Postedg.obj");
	Pareddg = Model();
	Pareddg.LoadModel("Models/pareddg.obj");

	skyboxFacesDay.push_back("Textures/Skybox/skybox_rgt.tga");
	skyboxFacesDay.push_back("Textures/Skybox/skybox_lft.tga");
	skyboxFacesDay.push_back("Textures/Skybox/skybox_btm.tga");
	skyboxFacesDay.push_back("Textures/Skybox/skybox_tp.tga");
	skyboxFacesDay.push_back("Textures/Skybox/skybox_bk.tga");
	skyboxFacesDay.push_back("Textures/Skybox/skybox_fr.tga");

	skyboxFacesNight.push_back("Textures/Skybox/skybox-night_rgt.tga");
	skyboxFacesNight.push_back("Textures/Skybox/skybox-night_lft.tga");
	skyboxFacesNight.push_back("Textures/Skybox/skybox-night_btm.tga");
	skyboxFacesNight.push_back("Textures/Skybox/skybox-night_tp.tga");
	skyboxFacesNight.push_back("Textures/Skybox/skybox-night_bk.tga");
	skyboxFacesNight.push_back("Textures/Skybox/skybox-night_fr.tga");

	skyboxDay = Skybox(skyboxFacesDay);
	skyboxNight = Skybox(skyboxFacesNight);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//Luces direccionales de noche y día
	mainLightDay = DirectionalLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, -1.0f);

	mainLightNight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.1f,
		0.0f, 0.0f, -1.0f);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	unsigned int spotLightCount = 0;

	//Luz de tacos
	staticPL[0] = PointLight(0.9608f, 0.6902f, 0.2549f,
		100.0f, 10.0f,
		100.0f, 25.0f, -150.0f,
		0.1f, 0.1f, 0.1f);
	pointLightCount++;

	//Luz de tortas
	staticPL[1] = PointLight(1.0f, 1.0f, 1.0f,
		50.0f, 10.0f,
		200.0f, 30.0f, -150.0f,
		0.2f, 0.2f, 0.2f);
	pointLightCount++;

	//Luz de sushi
	staticPL[2] = PointLight(0.0f, 0.0f, 1.0f,
		100.0f, 10.0f,
		130.0f, 25.0f, 150.0f,
		0.1f, 0.1f, 0.1f);
	pointLightCount++;

	//Luz de banderillas
	staticPL[3] = PointLight(0.9255f, 0.0f, 1.0f,
		50.0f, 10.0f,
		-100.0f, 5.0f, 150.0f,
		0.1f, 0.1f, 0.1f);
	pointLightCount++;

	//Luz pizzas
	staticSL[0] = SpotLight(1.0f, 0.0f, 0.0f,
		10.0f, 1.0f,
		-160.0f, 5.0f, -150.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.003f, 0.003f,
		20.0f);
	spotLightCount++;

	//Luz hotdogs
	staticSL[1] = SpotLight(1.0f, 1.0f, 0.0f,
		10.0f, 1.0f,
		-30.0f, 10.0f, -150.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.003f, 0.003f,
		20.0f);
	spotLightCount++;

	//Luz aguas
	staticSL[2] = SpotLight(1.0f, 1.0f, 1.0f,
		10.0f, 1.0f,
		-50.0f, 20.0f, 150.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.003f, 0.003f,
		20.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	movCoche = 0.0f;
	movHel = 0.0f;
	movHel1 = 0.0f;
	movOffset = 0.03f;//.01
	rotllanta = 0.05f;//0.05
	rotllanta1 = 0.5f;
	rotllanta2 = 6.1f;//0.05
	rotllantaOffset = 0.09f;
	//CORRECTO EL DE ABAJO
	sndPlaySound(TEXT("sound.wav"), SND_ASYNC | SND_LOOP);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime; ///Animación del coche, lo q estas vviendo esté estandarizado. 
		//deltaTime se mantenga constante la animación, si no se usara deltTime se vería la animación a difernete velocidad
		//dependiendo del equipo gráfico con el q se cuente
		///estamos diciedcon mientars sea menor a 30, decrementa el valor por el offset multiplicado por el deltaTime
		//vale .01m decrementandpo .01
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		if (currentTime - initialTime > 40) {
			dayCicleFlag = !dayCicleFlag;
			initialTime = time(NULL);
			currentTime = time(NULL);
		}
		else {
			currentTime = time(NULL);
		}

		if (avanza)
		{
			if (movCoche > -1.0f)
			{
				movCoche -= movOffset * deltaTime;
				//printf("avanza%f \n ",movCoche);
			}
			else {
				avanza = false;
			}
		}
		if (!avanza)
		{
			if (movCoche < 1.0f)
			{
				movCoche += movOffset * deltaTime;
			}
			else
			{
				avanza = true;
			}
		}
		if (avanza1)
		{
			if (movCoche1 > -5.0f)
			{
				movCoche1 -= movOffset * deltaTime;
				//printf("avanza%f \n ",movCoche);
			}
			else {
				avanza1 = false;
			}
		}
		if (!avanza1)
		{
			if (movHel < 15.0f)
			{
				movHel += movOffset * deltaTime;
			}
			else
			{
				avanza1 = true;
			}
		}
		if (!avanza1)
		{
			if (movHel > -25.0f)
			{
				movHel += movOffset * deltaTime;
			}
			else
			{
				avanza1 = true;
			}
		}
		if (!avanza2)
		{
			if (movHel1 < 1.0f)
			{
				movHel1 += movOffset * deltaTime;
			}
			else
			{
				avanza2 = true;
			}
		}
		if (!avanza2)
		{
			if (movHel1 > -1.0f)
			{
				movHel1 += movOffset * deltaTime;
			}
			else
			{
				avanza2 = true;
			}
		}
		//
		rotllanta += rotllantaOffset * deltaTime;

		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (dayCicleFlag)
			skyboxNight.DrawSkybox(camera.calculateViewMatrix(), projection);
		else
			skyboxDay.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		/*glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());*/

		//Encendido apagado y show de luces
		spotLightCount = 0;
		pointLightCount = 0;

		if (dayCicleFlag) {
			spotLights[spotLightCount] = staticSL[0]; //Añadimos luz de pizzas
			pizzasIndex = spotLightCount;
			spotLightCount++;
			spotLights[spotLightCount] = staticSL[1]; //Añadimos luz de hotdogs
			hotDIndex = spotLightCount;
			spotLightCount++;
			spotLights[spotLightCount] = staticSL[2]; //Añadimos luz de hotdogs
			aguasIndex = spotLightCount;
			spotLightCount++;

			pointLights[pointLightCount] = staticPL[0]; //Añadimos luz de tacos
			tacosIndex = pointLightCount;
			pointLightCount++;
			pointLights[pointLightCount] = staticPL[1]; //Añadimos luz de tortas
			tortasIndex = pointLightCount;
			pointLightCount++;
			pointLights[pointLightCount] = staticPL[2]; //Añadimos luz de sushi
			sushiIndex = pointLightCount;
			pointLightCount++;
			pointLights[pointLightCount] = staticPL[3]; //Añadimos luz de banderillas
			banderillasIndex = pointLightCount;
			pointLightCount++;
		}
		else {

		}

		//información al shader de fuentes de iluminación
		if (dayCicleFlag)
			shaderList[0].SetDirectionalLight(&mainLightNight);
		else
			shaderList[0].SetDirectionalLight(&mainLightDay);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		shaderList[0].SetPointLights(pointLights, pointLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(25.0f, 1.0f, 20.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//Food Truck Hot-Dogs
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, -8.0f, -150.0));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Fdhd.RenderModel();

		//Puesto Tacos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, -3.0f, -150.0));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuestoTacos1.RenderModel();
		//Trompo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f - movCoche, -3.0f, -150.0));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Trompo.RenderModel();

		//Puesto Tortas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, -3.0f, -150.0));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuestoTortas1.RenderModel();

		//Postedg
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, -3.0f, -150.0));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Postedg.RenderModel();

		//Bote
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, -1.0f, -50.0));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote.RenderModel();


		//Pareddg
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(190.0f, -3.0f, -30.0));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pareddg.RenderModel();
		//Tortas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, -3.0f, -150.0 - movCoche));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tortas.RenderModel();

		//Foodtruck Pízzas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -8.0f, -150.0));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Fdpi.RenderModel();

		//Letrero Pízzas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -8.0f, -150.0));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(-movHel, 0.0f - (3.5 * sin(glm::radians(rotllanta))), 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lpizza.RenderModel();

		//Don Gato
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, mainWindow.getorientacion() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dongato.RenderModel();

		//Sushi
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(130.0f, -2.0f, 150.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sushi.RenderModel(); +

		//platoSushi
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f - movCoche, -8.0f, 13.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Plato.RenderModel();

		//Carro de verduras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f - movCoche, -2.0f, 65.0));
		//model = glm::translate(model, glm::vec3(-movHel1, 3.0f + (3.5 * sin(glm::radians(rotllanta))), -movHel - 0.1 * rotllanta));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Verduras.RenderModel();

		//Carro de aguas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f - (2.0 * sin(5.0 * glm::radians(rotllanta))), 150.0));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Aguas.RenderModel();
		//Globo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f - (2.0 * sin(5.0 * glm::radians(rotllanta))), 150.0));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo.RenderModel();

		//Brochetas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -2.0f, 150.0));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, -185 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Brochetas.RenderModel();

		//Teatro
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, 0.0f, 20.0));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, -185 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Teatro.RenderModel();

		//textura con movimiento
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetu += 0.001;
		toffsetv += 0.0;
		//para que no se desborde la variable
		if (toffsetu > 1.0)
			toffsetu = 0.0;//eje hacia arriba
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
		//printf("\ntfosset %f \n", toffsetu);
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetu, toffsetv);//2 valores, el incremento hace q las flechas se vean animadas
		//las felchas se mueven es un plano y e mueven las coordenadas de texturizado
		//ecuación del peralte
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 15.0f, -190.0));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//toffset=glm::vec2(0.0f,0.0f);
		//AgaveTexture.UseTexture();
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[3]->RenderMesh();
		glDisable(GL_BLEND);
		meshList[3]->RenderMesh();
		///
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, 15.0f, -140.0));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		toffset = glm::vec2(0.0f, 0.0f);
		//AgaveTexture.UseTexture();
		FlechaTexture1.UseTexture();
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();
		//
		glDisable(GL_BLEND);

		//Toroide
		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		model = glm::translate(model, glm::vec3(-160.0f, -2.0f, 150.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[5]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
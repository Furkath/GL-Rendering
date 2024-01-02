#include <iostream>
#include <fstream>
#include <sstream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <SOIL/SOIL.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//Reading the obj file
//int v_num=0; //点
//int vn_num=0;//法线
//int f_num=0; //面
//int vt_num=0;//纹理坐标
void getLineNum(const std::string & sFileName, int& v_num, int& vn_num, int& vt_num, int& f_num)
{
 //int v_num;
 //int vn_num;
 //int f_num;
 std::ifstream infile(sFileName.c_str());
 std::string sline;
 v_num=vn_num=f_num=vt_num=0;
 getline(infile,sline);
 //while(getline(infile,sline))
 while(sline.size()!=0)
 {//
  if(sline[0]=='v')
  {
   if(sline[1]=='n')
    vn_num++;
   else if(sline[1]!='t')//==' '
    v_num++;
   else if(sline[1]=='t')
    vt_num++;
  }
  else if(sline[0]=='f')
       f_num++;
     getline(infile,sline);
 }
 infile.close();
 std::cout<<v_num<<std::endl;
 std::cout<<vn_num<<std::endl;
 std::cout<<vt_num<<std::endl;
 std::cout<<f_num<<std::endl;
 //return f_num;
}

void readObjFile(std::string addrstr, GLfloat* Arr_read, int v_num, int vn_num, int vt_num, int f_num)
{
        std::string s1;
        GLfloat f2,f3,f4;
        GLfloat **vArr; //点
        GLfloat **vnArr;//法线
  	GLfloat **vtArr;//纹理坐标
        //GLfloat Arr_read[f_num*18];
vArr=new GLfloat*[v_num];
for (int i=0;i<v_num;i++)
{
  vArr[i]=new GLfloat[3];
}
vnArr=new GLfloat*[vn_num];
for (int i=0;i<vn_num;i++)
{
  vnArr[i]=new GLfloat[3];
}
vtArr=new GLfloat*[vt_num];
for (int i=0;i<vt_num;i++)
{
  vtArr[i]=new GLfloat[2];
}
std::ifstream infile(addrstr.c_str());
std::string sline;//
int ii=0,jj=0,zz=0,kk=0;
while(getline(infile,sline))
{
if(sline[0]=='v')
{
  if(sline[1]=='n')//vn
  {
    std::istringstream sin(sline);
    sin>>s1>>f2>>f3>>f4;
    vnArr[ii][0]=f2;
    vnArr[ii][1]=f3;
    vnArr[ii][2]=f4;
    ii++;
  }
  else if(sline[1]!='t')//v
  {
    std::istringstream sin(sline);
    sin>>s1>>f2>>f3>>f4;
    vArr[jj][0]=f2;
    vArr[jj][1]=f3;
    vArr[jj][2]=f4;
    jj++;
  }
  else if(sline[1]=='t')//vt
  {
    std::istringstream sin(sline);
    sin>>s1>>f2>>f3;
    vtArr[zz][0]=f2;
    vtArr[zz][1]=f3;
    zz++;
  }	 
}
if (sline[0]=='f') //面
{
  std::istringstream in(sline);
  GLuint a;
  in>>s1;//去掉f
  int i,k;
  for(i=0;i<3;i++)
  {
    in>>s1;
    //std::cout<<s1<<std::endl;
    //
    a=0;
    for(k=0;s1[k]!='/';k++)
    {
      //std::cout<<k<<std::endl;
      a=a*10+(s1[k]-48);
    }
    //std::cout<<"ok"<<std::endl;
    Arr_read[24*kk+8*i+0]=vArr[a-1][0];
    Arr_read[24*kk+8*i+1]=vArr[a-1][1];
    Arr_read[24*kk+8*i+2]=vArr[a-1][2];
    a=0;
    //std::cout<<"ok"<<std::endl;
    //std::cout<<"k is "<<k<<std::endl;
    for(k=k+1;s1[k]!='/';k++)
    {
      a=a*10+(s1[k]-48);
    //std::cout<<k<<std::endl;
    }
    Arr_read[24*kk+8*i+6]=vtArr[a-1][0];
    Arr_read[24*kk+8*i+7]=vtArr[a-1][1];
    a=0;
    //std::cout<<"and k is "<<k<<std::endl;
    for(k=k+1;s1[k];k++)
    {
      a=a*10+(s1[k]-48);
      //std::cout<<"a : "<<a<<std::endl;
    }
    Arr_read[24*kk+8*i+3]=vnArr[a-1][0];
    Arr_read[24*kk+8*i+4]=vnArr[a-1][1];
    Arr_read[24*kk+8*i+5]=vnArr[a-1][2];
    //std::cout<<"ok"<<std::endl;
  }
  kk++;
  }
}
//return Arr_read;
}


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void do_movement();

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Window dimensions
const GLuint WIDTH = 3328, HEIGHT = 1872;

GLfloat mixValue = 0.2f;

bool keys[1024];

glm::mat4 view = glm::mat4(1.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat lastX = WIDTH / 2, lastY = HEIGHT / 2;
bool firstMouse;
GLfloat yaw = -90.0f;	
GLfloat pitch = 0.0f;

GLfloat aspect = 45.0f;

const char *vs_shadow = "#version 450 core\n"
"layout (location=0) in vec3 aPos;\n"
"uniform mat4 lightSpace;\n"
"uniform mat4 model;\n"
"void main()\n"
"{\n"
"	gl_Position = lightSpace * model * vec4(aPos, 1.0);\n"
"}\0";
const char *fs_shadow = "#version 450 core\n"
"void main()\n"
"{\n"
"}\0";

const char *vs_light = "#version 450 core\n"
"layout (location = 0) in vec3 aPos;\n"
//"out vec3 FragPos;\n"
//out vec3 Normal;
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
//"    FragPos = vec3(model * vec4(aPos, 1.0));\n"
    //Normal = aNormal;  
"    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"}\0";

    //片段着色器 light.frag
const char *fs_light = "#version 450 core\n"
    "out vec4 FragColor;\n"
    //uniform vec3 lightColor;
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.9f, 0.8f, 1.0);\n"
    "}\0";

const char *vs_phone ="#version 450 core\n"
    "layout (location = 0) in vec3 vposition;\n"
    "layout (location = 1) in vec3 vnormal;\n"
    //"layout (location = 2) in vec3 vcolor;\n"
    "layout (location = 2) in vec2 texCoord;\n"
    "#define NUM_POINT_LIGHTS 2\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 lightSpace[NUM_POINT_LIGHTS];\n" //added
    "out vec2 TexCoord;\n"
    "out vec3 position;\n"
    "out vec3 Normal;\n"
    "out vec4 Pos_lightspace[NUM_POINT_LIGHTS];\n" //added
    //"out vec4 fscolor;\n"

    "void main()\n"
    "{\n"
    "   vec4 v1 = model * vec4(vposition, 1.0f);\n"
    "   vec4 v2 = vec4(v1.xyz/v1.w, 1.0);\n"
    "   gl_Position = projection * view * v1;\n"
    //"   gl_Position = projection * view * model * vec4(position, 1.0f);\n"
    "   position = vec3(v2.xyz);\n"
    "   Normal = mat3(transpose(inverse(model)))*vnormal;\n"
    "   TexCoord = vec2(texCoord.x , 1.0 - texCoord.y);\n"//1.0 - texCoord.y);\n"
    "   for(int i=0;i<NUM_POINT_LIGHTS;++i)\n"
    "	{\n"
    "       Pos_lightspace[i] = lightSpace[i] * v2;\n" //added
    "	}\n"
    //"   Pos_lightspace = lightSpace * v2;\n" //added
    "}\0";
const char *fs_phone = "#version 450 core\n"
    "struct Material{\n"
    "        sampler2D diffuse_texture;\n"
    "        sampler2D specular_texture;\n"
    "        float shininess;\n"
    "};\n"
    "struct DirLight{\n"
    "        vec3 direction;\n"
    "        vec3 ambient;\n"
    "        vec3 diffuse;\n"
    "        vec3 specular;\n"
    "};\n"
    "struct PointLight{\n"
    "        vec3 position;\n"
    "        float constant;\n"
    "        float linear;\n"
    "        float quadratic;\n"
    "        vec3 ambient;\n"
    "        vec3 diffuse;\n"
    "        vec3 specular;\n"
    "};\n"
    "struct SpotLight{\n"
    "        vec3 position;\n"
    "        vec3 direction;\n"
    "        float cutoff;\n"
    "        float outercutoff;\n"
    "        float constant;\n"
    "        float linear;\n"
    "        float quadratic;\n"
    "        vec3 ambient;\n"
    "        vec3 diffuse;\n"
    "        vec3 specular;\n"
    "};\n"
    "#define NUM_POINT_LIGHTS 2\n"
    "#define NUM_TRANS_OBJ 1\n"   //add for trans
    "in vec3 position;\n"
    "in vec3 Normal;\n"
    "in vec2 TexCoord;\n"
    "in vec4 Pos_lightspace[NUM_POINT_LIGHTS];\n"  //added
    "out vec4 color;\n"
    "uniform vec3 viewPos;\n"
    "uniform DirLight dirLight;\n"
    "uniform PointLight pointLights[NUM_POINT_LIGHTS];\n"
    "uniform SpotLight spotLight;\n"
    "uniform Material material;\n"
    "uniform sampler2D depthMap[NUM_POINT_LIGHTS+NUM_TRANS_OBJ];\n"  //added
    "uniform float transparency;\n" //added for trans  
    //"uniform float trans_a;\n"				                                             
    "uniform vec3 trans_color;\n"   //add for trans
    //"// Function prototypes
    "vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);\n"
    "vec3 CalcPointLight(int i, PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n"
    "vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n"
    
    "float ShadowCalc(int i, vec4 Pos_lightspace, vec3 norm_, vec3 lifgtdir_)\n"
    "{\n"
    "       vec3 projcoord = Pos_lightspace.xyz/Pos_lightspace.w;\n"
    "       projcoord = projcoord*0.5+0.5;\n"
    "       float current = projcoord.z;\n"
    "       float bias = max(0.00001, 0.0004*pow(1.0-dot(norm_, lifgtdir_ ), 0.1) );\n"
    "       float shadow = 0.0;\n"
    "       vec2 texelsize = 1.0/textureSize(depthMap[i],0);\n"
    "       for(int x =-1;x<=1;++x)\n"
    "       {\n"
    "           for(int y = -1; y <= 1; ++y)\n"
    "           {\n"
    "           float pcfDepth = texture(depthMap[i], projcoord.xy + vec2(x, y) * texelsize).r;\n"
    "           shadow += current - bias > pcfDepth ? 1.0 : 0.0;\n"
    "           }\n" 
    "       }\n"
    "       shadow /= 9.0;\n"
    "       return shadow;\n"
    "}\n"

    "void main()\n"
    "{\n"
    //"    // Properties
    "    vec3 norm = normalize(Normal);\n"
    "    vec3 viewDir = normalize(viewPos - position);\n"
    
    //"    // Phase 1: Directional lighting
    "    vec3 result = CalcDirLight(dirLight, norm, viewDir);\n"
    //"    // Phase 2: Point lights
    "    for(int i = 0; i < NUM_POINT_LIGHTS; i++)\n"
    "        result += CalcPointLight(i, pointLights[i], norm, position, viewDir);\n"
    //"    // Phase 3: Spot light
    "    result += CalcSpotLight(spotLight, norm, position, viewDir);\n"
    
    "    color = vec4(result, 1.0);\n"
    "}\n"
    
    //"// Calculates the color when using a directional light.
    "vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)\n"
    "{\n"
    "    vec3 lightDir = normalize(-light.direction);\n"
    //"    // Diffuse shading
    "    float diff = max(dot(normal, lightDir), 0.0);\n"
    //"    // Specular shading
    "    vec3 reflectDir = reflect(-lightDir, normal);\n"
    "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
    //"    // Combine results
    "    vec3 ambient = light.ambient * vec3(texture(material.diffuse_texture, TexCoord));\n"
    "    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse_texture, TexCoord));\n"
    "    vec3 specular = light.specular * spec * vec3(texture(material.specular_texture, TexCoord));\n"
    "    return (ambient + diffuse + specular);\n"
    "}\n"
    
    //"// Calculates the color when using a point light.
    "vec3 CalcPointLight(int i, PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\n"
    "{\n"
    "    vec3 lightDir = normalize(light.position - fragPos);\n"
    //"    // Diffuse shading
    "    float diff = max(dot(normal, lightDir), 0.0);\n"
    //"    // Specular shading
    "    vec3 reflectDir = reflect(-lightDir, normal);\n"
    "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
    //"    // Attenuation
    "    float distance = length(light.position - fragPos);\n"
    "    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n"
    //"    // Combine results
    //"    vec3 ambient = light.ambient * vec3(texture(material.diffuse_texture, TexCoord));\n"
    //"    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse_texture, TexCoord));\n"
    //"    vec3 specular = light.specular * spec * vec3(texture(material.specular_texture, TexCoord));\n"
    "    float shadow;\n"
    "    shadow = ShadowCalc(i, Pos_lightspace[i], normal, lightDir);\n"
    "    float shadow_trans;\n"
    "    vec3 thiscolor;\n"
    "    if (i==NUM_TRANS_OBJ)\n"
    "    {\n"
    "           shadow_trans = ShadowCalc(i+NUM_TRANS_OBJ, Pos_lightspace[i], normal, lightDir);\n"
    //"                 //if (shadow_trans>0.001 && shadow<0.999)\n" 
                //{
    "           vec3 amb_thiscolor=(1.0-transparency)*(light.ambient*(1.0-transparency)+transparency*trans_color);\n"
    "           vec3 diff_thiscolor=(1.0-transparency)*(light.diffuse*(1.0-transparency)+transparency*trans_color);\n"
    "           vec3 spec_thiscolor=(1.0-transparency)*(light.specular*(1.0-transparency)+transparency*trans_color);\n"
    "           vec3 ambient = amb_thiscolor * vec3(texture(material.diffuse_texture, TexCoord));\n"
    "           vec3 diffuse = diff_thiscolor * diff * vec3(texture(material.diffuse_texture, TexCoord));\n"
    "           vec3 specular = spec_thiscolor * spec * vec3(texture(material.specular_texture, TexCoord));\n"
    "           ambient *= attenuation;\n"
    "           diffuse *= attenuation;\n"
    "           specular *= attenuation;\n"
    "           thiscolor=ambient + diffuse + specular;\n"
                //}
    "    }\n"
         //else thiscolor=thecolor;
         //if (i==NUM_TRANS_OBJ) vec3 thiscolor=(1.0-transparency)*(thecolor*(1.0-transparency)+transparency*trans_color);
    "    vec3 ambient = light.ambient * vec3(texture(material.diffuse_texture, TexCoord));\n"
    "    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse_texture, TexCoord));\n"
    "    vec3 specular = light.specular * spec * vec3(texture(material.specular_texture, TexCoord));\n"
    "    ambient *= attenuation;\n"
    "    diffuse *= attenuation;\n"
    "    specular *= attenuation;\n"
    //"    float shadow;\n"
    //"    shadow = ShadowCalc(i, Pos_lightspace[i], normal, lightDir);\n"
         //float shadow_trans;
         //if (i==NUM_TRANS_OBJ) shadow_trans = ShadowCalc(i+NUM_TRANS_OBJ, Pos_lightspace[i], normal, lightDir);
    "    if (i==NUM_TRANS_OBJ)\n"
    "    return ( thiscolor*shadow_trans*(1.0-shadow) + (1.0-shadow_trans)*(1.0-shadow)*(ambient + diffuse + specular) + shadow*0.25*ambient );\n"
    "    else\n"
    "    return ( (1.0-shadow*0.75) * ambient + (1.0-shadow)*(diffuse + specular));\n"
         //else if(shadow<1.0)
         //return 
    "}\n"

    
    "vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\n"
    "{\n"
    "    vec3 lightDir = normalize(light.position - fragPos);\n"
    //"    // Diffuse shading
    "    float diff = max(dot(normal, lightDir), 0.0);\n"
    //"    // Specular shading
    "    vec3 reflectDir = reflect(-lightDir, normal);\n"
    "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
    //"    // Attenuation
    "    float distance = length(light.position - fragPos);\n"
    "    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n"
    //"    // Spotlight intensity
    "    float theta = dot(lightDir, normalize(-light.direction));\n"
    "    float epsilon = light.cutoff - light.outercutoff;\n"
    "    float intensity = clamp((theta - light.outercutoff) / epsilon, 0.0, 1.0);\n"
    //"    // Combine results
    "    vec3 ambient = light.ambient * vec3(texture(material.diffuse_texture, TexCoord));\n"
    "    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse_texture, TexCoord));\n"
    "    vec3 specular = light.specular * spec * vec3(texture(material.specular_texture, TexCoord));\n"
    "    ambient *= attenuation * intensity;\n"
    "    diffuse *= attenuation * intensity;\n"
    "    specular *= attenuation * intensity;\n"
    "    return (ambient + diffuse + specular);\n"
    "}\n";
   
//phone_shading_no_texture----------------------------------------------------
const char *vs_phone_no_text ="#version 450 core\n"
    "layout (location = 0) in vec3 vposition;\n"
    "layout (location = 1) in vec3 vnormal;\n"
    //"layout (location = 2) in vec3 vcolor;\n"
    //"layout (location = 2) in vec2 texCoord;\n"
    "#define NUM_POINT_LIGHTS 2\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 lightSpace[NUM_POINT_LIGHTS];\n" //added
    //"out vec2 TexCoord;\n"
    "out vec3 position;\n"
    "out vec3 Normal;\n"
    //"out vec4 fscolor;\n"
    "out vec4 Pos_lightspace[NUM_POINT_LIGHTS];\n" //added

    "void main()\n"
    "{\n"
    "   vec4 v1 = model * vec4(vposition, 1.0f);\n"
    "   vec4 v2 = vec4(v1.xyz/v1.w, 1.0);\n"
    "   gl_Position = projection * view * v1;\n"
    //"   gl_Position = projection * view * model * vec4(position, 1.0f);\n"
    "   position = vec3(v2.xyz);\n"
    "   Normal = mat3(transpose(inverse(model)))*vnormal;\n"
    //"   TexCoord = vec2(texCoord.x , 1.0 - texCoord.y);\n"//1.0 - texCoord.y);\n"
    "   for(int i=0;i<NUM_POINT_LIGHTS;++i)\n"
    "   {\n"
    "       Pos_lightspace[i] = lightSpace[i] * v2;\n" //added
    "   }\n"
    //"   Pos_lightspace = lightSpace * v2;\n" //added
    "}\0";

const char *fs_phone_no_text = "#version 450 core\n"
    "struct Material{\n"
    //"        sampler2D diffuse_texture;\n"
    //"        sampler2D specular_texture;\n"
    "        float shininess;\n"
    "};\n"
    "struct DirLight{\n"
    "        vec3 direction;\n"
    "        vec3 ambient;\n"
    "        vec3 diffuse;\n"
    "        vec3 specular;\n"
    "};\n"
    "struct PointLight{\n"
    "        vec3 position;\n"
    "        float constant;\n"
    "        float linear;\n"
    "        float quadratic;\n"
    "        vec3 ambient;\n"
    "        vec3 diffuse;\n"
    "        vec3 specular;\n"
    "};\n"
    "struct SpotLight{\n"
    "        vec3 position;\n"
    "        vec3 direction;\n"
    "        float cutoff;\n"
    "        float outercutoff;\n"
    "        float constant;\n"
    "        float linear;\n"
    "        float quadratic;\n"
    "        vec3 ambient;\n"
    "        vec3 diffuse;\n"
    "        vec3 specular;\n"
    "};\n"
    "#define NUM_POINT_LIGHTS 2\n"
    "#define NUM_TRANS_OBJ 1\n"   //add for trans
    "in vec3 position;\n"
    "in vec3 Normal;\n"
    //"in vec2 TexCoord;\n"
    "in vec4 Pos_lightspace[NUM_POINT_LIGHTS];\n"  //added
    "out vec4 color;\n"
    "uniform vec3 viewPos;\n"
    "uniform DirLight dirLight;\n"
    "uniform PointLight pointLights[NUM_POINT_LIGHTS];\n"
    "uniform SpotLight spotLight;\n"
    "uniform Material material;\n"
    "uniform vec3 thecolor;\n"
    "uniform sampler2D depthMap[NUM_POINT_LIGHTS+NUM_TRANS_OBJ];\n"  //added
    "uniform float transparency;\n" //added for trans	
    "uniform float trans_a;\n"					
    "uniform vec3 trans_color;\n"   //add for trans 

    "float ShadowCalc(int i, vec4 Pos_lightspace, vec3 norm_, vec3 lifgtdir_)\n"
    "{\n"
    "       vec3 projcoord = Pos_lightspace.xyz/Pos_lightspace.w;\n"
    "       projcoord = projcoord*0.5+0.5;\n"
    "       float current = projcoord.z;\n"
    "       float bias = max(0.00001, 0.0004*pow(1.0-dot(norm_, lifgtdir_ ), 0.1) );\n"
    "	    float shadow = 0.0;\n"
    "	    vec2 texelsize = 1.0/textureSize(depthMap[i],0);\n"
    "	    for(int x =-1;x<=1;++x)\n"
    "	    {\n"
    "	 	for(int y = -1; y <= 1; ++y)\n"
    "		{\n"
    "		float pcfDepth = texture(depthMap[i], projcoord.xy + vec2(x, y) * texelsize).r;\n"
                //float pcf_tran = texture(depthMap[i], projcoord.xy + vec2(x, y) * texelsize).r;
    "		shadow += current - bias > pcfDepth ? 1.0 : 0.0;\n"
    "   	}\n"    
    " 	    }\n"
    "       shadow /= 9.0;\n"
    "       return shadow;\n"
    "}\n"

    //"// Function prototypes
    "vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);\n"
    "vec3 CalcPointLight(int i, PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n"
    "vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n"
    //"vec3 thecolor = vec3(255, 227, 132)/255;\n"
    "void main()\n"
    "{\n"
    //"    // Properties
    "    vec3 norm = normalize(Normal);\n"
    "    vec3 viewDir = normalize(viewPos - position);\n"
    //"	 if (transparency < 0.99)"
    //"    // Phase 1: Directional lighting
    "    vec3 result = CalcDirLight(dirLight, norm, viewDir);\n"
    //"    // Phase 2: Point lights
    "    for(int i = 0; i < NUM_POINT_LIGHTS; i++)\n"
    "        result += CalcPointLight(i, pointLights[i], norm, position, viewDir);\n"
    //"    // Phase 3: Spot light
    "    result += CalcSpotLight(spotLight, norm, position, viewDir);\n"
    "    color = vec4(result, trans_a);\n"
    "}\n"
    
    //"// Calculates the color when using a directional light.
    "vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)\n"
    "{\n"
    "    vec3 lightDir = normalize(-light.direction);\n"
    //"    // Diffuse shading
    "    float diff = max(dot(normal, lightDir), 0.0);\n"
    //"    // Specular shading
    "    vec3 reflectDir = reflect(-lightDir, normal);\n"
    "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
    //"    // Combine results
    "    vec3 ambient = light.ambient * thecolor;\n"
    "    vec3 diffuse = light.diffuse * diff * thecolor;\n"
    "    vec3 specular = light.specular * spec * thecolor;\n"
    "    return (ambient + diffuse + specular);\n"
    "}\n"
    
    //"// Calculates the color when using a point light.
    "vec3 CalcPointLight(int i, PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\n"
    "{\n"
    "    vec3 lightDir = normalize(light.position - fragPos);\n"
    //"    // Diffuse shading
    "    float diff = max(dot(normal, lightDir), 0.0);\n"
    //"    // Specular shading
    "    vec3 reflectDir = reflect(-lightDir, normal);\n"
    "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
    //"    // Attenuation
    "    float distance = length(light.position - fragPos);\n"
    "    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n"
    //"    // Combine results
    "    float shadow;\n"
    "    shadow = ShadowCalc(i, Pos_lightspace[i], normal, lightDir);\n"
    "    float shadow_trans;\n"
    "	 vec3 thiscolor;\n"
    "    if (i==NUM_TRANS_OBJ)\n"
    "	 {\n"
    "	 	shadow_trans = ShadowCalc(i+NUM_TRANS_OBJ, Pos_lightspace[i], normal, lightDir);\n"
    //"         	//if (shadow_trans>0.001 && shadow<0.999)\n" 
    		//{
    "		vec3 amb_thiscolor=(1.0-transparency)*(light.ambient*(1.0-transparency)+transparency*trans_color);\n"
    "           vec3 diff_thiscolor=(1.0-transparency)*(light.diffuse*(1.0-transparency)+transparency*trans_color);\n"
    "           vec3 spec_thiscolor=(1.0-transparency)*(light.specular*(1.0-transparency)+transparency*trans_color);\n"
    "           vec3 ambient = amb_thiscolor * thecolor;\n"            
    "           vec3 diffuse = diff_thiscolor * diff * thecolor;\n"
    "           vec3 specular = spec_thiscolor * spec * thecolor;\n"
    "           ambient *= attenuation;\n"
    "           diffuse *= attenuation;\n"
    "           specular *= attenuation;\n"
    "           thiscolor=ambient + diffuse + specular;\n"
	        //}
    "	 }\n"
	 //else thiscolor=thecolor;
         //if (i==NUM_TRANS_OBJ) vec3 thiscolor=(1.0-transparency)*(thecolor*(1.0-transparency)+transparency*trans_color);
    "    vec3 ambient = light.ambient * thecolor;\n"
    "    vec3 diffuse = light.diffuse * diff * thecolor;\n"
    "    vec3 specular = light.specular * spec * thecolor;\n"
    "    ambient *= attenuation;\n"
    "    diffuse *= attenuation;\n"
    "    specular *= attenuation;\n"
    //"    float shadow;\n"
    //"    shadow = ShadowCalc(i, Pos_lightspace[i], normal, lightDir);\n"
         //float shadow_trans;
         //if (i==NUM_TRANS_OBJ) shadow_trans = ShadowCalc(i+NUM_TRANS_OBJ, Pos_lightspace[i], normal, lightDir);
    "	 if (i==NUM_TRANS_OBJ)\n"
    "    return ( thiscolor*shadow_trans*(1.0-shadow) + (1.0-shadow_trans)*(1.0-shadow)*(ambient + diffuse + specular) + shadow*0.25*ambient );\n"
    "    else\n"
    "    return ( (1.0-shadow*0.75) * ambient + (1.0-shadow)*(diffuse + specular));\n"
         //else if(shadow<1.0)
	 //return 
    "}\n"
    
    "vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\n"
    "{\n"
    "    vec3 lightDir = normalize(light.position - fragPos);\n"
    //"    // Diffuse shading
    "    float diff = max(dot(normal, lightDir), 0.0);\n"
    //"    // Specular shading
    "    vec3 reflectDir = reflect(-lightDir, normal);\n"
    "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
    //"    // Attenuation
    "    float distance = length(light.position - fragPos);\n"
    "    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n"
    //"    // Spotlight intensity
    "    float theta = dot(lightDir, normalize(-light.direction));\n"
    "    float epsilon = light.cutoff - light.outercutoff;\n"
    "    float intensity = clamp((theta - light.outercutoff) / epsilon, 0.0, 1.0);\n"
    //"    // Combine results
    "    vec3 ambient = light.ambient * thecolor;\n"
    "    vec3 diffuse = light.diffuse * diff * thecolor;\n"
    "    vec3 specular = light.specular * spec * thecolor;\n"
    "    ambient *= attenuation * intensity;\n"
    "    diffuse *= attenuation * intensity;\n"
    "    specular *= attenuation * intensity;\n"
    "    return (ambient + diffuse + specular);\n"
    "}\n";
//----------------------------------------------------------------------------


// The MAIN function, from here start
int main()
{
    int num_obj=3;
    int v_num[num_obj]={0}; //点的数量
    int vn_num[num_obj]={0};//法线的数量
    int f_num[num_obj]={0}; //面的数量 
    int vt_num[num_obj]={0};//纹理的数量
    //int f_total=0; 
    getLineNum("ball.obj", v_num[0], vn_num[0], vt_num[0], f_num[0]);
    getLineNum("zhui.obj", v_num[1], vn_num[1], vt_num[1], f_num[1]);
    getLineNum("zhu.obj", v_num[2], vn_num[2], vt_num[2], f_num[2]);
    //std::cout<<"--------------------"<<std::endl;
    //std::cout<<f_num[0]<<f_num[1]<<f_num[2]<<std::endl;
    int f_total=f_num[0]+f_num[1]+f_num[2];
    //GLuint f_num_ball=f_num;
    GLfloat verticesObj[f_total*24];
    readObjFile("ball.obj", verticesObj, v_num[0], vn_num[0], vt_num[0], f_num[0]);
    //for(int I=0;I<1000;I++) std::cout<<verticesObj[I]<<std::endl;
    //getLineNum("zhui.obj");
    //GLuint f_num_zhui=f_num;
    //GLfloat verticeszhui[f_num_zhui*24];
    readObjFile("zhui.obj", &verticesObj[24*f_num[0]], v_num[1], vn_num[1], vt_num[1], f_num[1]);
    //getLineNum("zhu.obj");
    //GLuint f_num_zhu=f_num;
    //GLfloat verticeszhu[f_num_zhu*24];
    readObjFile("zhu.obj", &verticesObj[24*(f_num[0]+f_num[1])], v_num[2], vn_num[2], vt_num[2], f_num[2]);
    //std::cout<<"------------------------------------"<<std::endl;
    //for(int I=0;I<1000;I++) std::cout<<verticesObj[I]<<std::endl;
    //GLfloat verticesObj[(f_num_ball+f_num_zhui+f_num_zhu)*24];
    //for(int I=0;I<1000;I++) std::cout<<verticesball[I]<<std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "CG_Final_Project", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);
    // 开启深度测试
    glEnable(GL_DEPTH_TEST);

    // Build and compile shader programs
    GLuint vertex_sha, fragment_sha;
    GLint success_flag;
    GLchar infoLog_char[512];
    vertex_sha=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_sha, 1, &vs_phone, NULL);
    glCompileShader(vertex_sha);
    glGetShaderiv(vertex_sha, GL_COMPILE_STATUS, &success_flag);
                if (!success_flag)
                {
                        glGetShaderInfoLog(vertex_sha, 512, NULL, infoLog_char);
                        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog_char << std::endl;
                }
    fragment_sha = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_sha, 1, &fs_phone, NULL);
    glCompileShader(fragment_sha);
    glGetShaderiv(fragment_sha, GL_COMPILE_STATUS, &success_flag);
                if (!success_flag)
                {
                        glGetShaderInfoLog(fragment_sha, 512, NULL, infoLog_char);
                        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog_char << std::endl;
                }
    GLuint shaderProgram_int3 = glCreateProgram();
    glAttachShader(shaderProgram_int3, vertex_sha);
    glAttachShader(shaderProgram_int3, fragment_sha);
    glLinkProgram(shaderProgram_int3);
    glGetProgramiv(shaderProgram_int3, GL_LINK_STATUS, &success_flag);
    if (!success_flag) {
        glGetProgramInfoLog(shaderProgram_int3, 512, NULL, infoLog_char);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog_char << std::endl;
    }
    glDeleteShader(vertex_sha);
    glDeleteShader(fragment_sha);
//---------------------------------------Phone_Shader

//-------------------------shadow shader-------------
    vertex_sha=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_sha, 1, &vs_shadow, NULL);
    glCompileShader(vertex_sha);
    glGetShaderiv(vertex_sha, GL_COMPILE_STATUS, &success_flag);
                if (!success_flag)
                {
                        glGetShaderInfoLog(vertex_sha, 512, NULL, infoLog_char);
                        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog_char << std::endl;
                }
    fragment_sha = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_sha, 1, &fs_shadow, NULL);
    glCompileShader(fragment_sha);
    glGetShaderiv(fragment_sha, GL_COMPILE_STATUS, &success_flag);
                if (!success_flag)
                {
                        glGetShaderInfoLog(fragment_sha, 512, NULL, infoLog_char);
                        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog_char << std::endl;
                }
    GLuint shaderProgram_shadow = glCreateProgram();
    glAttachShader(shaderProgram_shadow, vertex_sha);
    glAttachShader(shaderProgram_shadow, fragment_sha);
    glLinkProgram(shaderProgram_shadow);
    glGetProgramiv(shaderProgram_shadow, GL_LINK_STATUS, &success_flag);
    if (!success_flag) {
        glGetProgramInfoLog(shaderProgram_shadow, 512, NULL, infoLog_char);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog_char << std::endl;
    }
    glDeleteShader(vertex_sha);
    glDeleteShader(fragment_sha);
//---------------------------------------------------

//Then use the light shader:
    vertex_sha=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_sha, 1, &vs_light, NULL);
    glCompileShader(vertex_sha);
    glGetShaderiv(vertex_sha, GL_COMPILE_STATUS, &success_flag);
                if (!success_flag)
                {
                        glGetShaderInfoLog(vertex_sha, 512, NULL, infoLog_char);
                        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog_char << std::endl;
                }
    fragment_sha = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_sha, 1, &fs_light, NULL);
    glCompileShader(fragment_sha);
    glGetShaderiv(fragment_sha, GL_COMPILE_STATUS, &success_flag);
                if (!success_flag)
                {
                        glGetShaderInfoLog(fragment_sha, 512, NULL, infoLog_char);
                        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog_char << std::endl;
                }
    GLuint shaderProgram_intL = glCreateProgram();
    glAttachShader(shaderProgram_intL, vertex_sha);
    glAttachShader(shaderProgram_intL, fragment_sha);
    glLinkProgram(shaderProgram_intL);
    glGetProgramiv(shaderProgram_intL, GL_LINK_STATUS, &success_flag);
    if (!success_flag) {
        glGetProgramInfoLog(shaderProgram_intL, 512, NULL, infoLog_char);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog_char << std::endl;
    }
    glDeleteShader(vertex_sha);
    glDeleteShader(fragment_sha);
//-------------------------------    

//Then use the phone_no_text shader:
    vertex_sha=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_sha, 1, &vs_phone_no_text, NULL);
    glCompileShader(vertex_sha);
    glGetShaderiv(vertex_sha, GL_COMPILE_STATUS, &success_flag);
                if (!success_flag)
                {
                        glGetShaderInfoLog(vertex_sha, 512, NULL, infoLog_char);
                        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog_char << std::endl;
                }
    fragment_sha = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_sha, 1, &fs_phone_no_text, NULL);
    glCompileShader(fragment_sha);
    glGetShaderiv(fragment_sha, GL_COMPILE_STATUS, &success_flag);
                if (!success_flag)
                {
                        glGetShaderInfoLog(fragment_sha, 512, NULL, infoLog_char);
                        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog_char << std::endl;
                }
    GLuint shaderProgram_notext = glCreateProgram();
    glAttachShader(shaderProgram_notext, vertex_sha);
    glAttachShader(shaderProgram_notext, fragment_sha);
    glLinkProgram(shaderProgram_notext);
    glGetProgramiv(shaderProgram_notext, GL_LINK_STATUS, &success_flag);
    if (!success_flag) {
        glGetProgramInfoLog(shaderProgram_notext, 512, NULL, infoLog_char);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog_char << std::endl;
    }
    glDeleteShader(vertex_sha);
    glDeleteShader(fragment_sha);
//------------------------------- 

    // 立方体顶点数据（位置坐标+法现+纹理坐标）
    GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,  0.57735027f, -0.57735027f, -0.57735027f,  0.0f, 0.1339746f, 
     0.5f, -0.5f,  0.5f,  0.57735027f, -0.57735027f, -0.57735027f,  1.0f, 0.1339746f,
     0.5f,  0.5f, -0.5f,  0.57735027f, -0.57735027f, -0.57735027f,  0.5f, 1.0f,

     0.5f,  0.5f, -0.5f, -0.57735027f,  0.57735027f, -0.57735027f,  1.0f, 0.8660254f,
    -0.5f,  0.5f,  0.5f, -0.57735027f,  0.57735027f, -0.57735027f,  0.0f, 0.8660254f,
    -0.5f, -0.5f, -0.5f, -0.57735027f,  0.57735027f, -0.57735027f,  0.5f, 0.0f,
     
     0.5f,  0.5f, -0.5f,  0.57735027f,  0.57735027f,  0.57735027f,  1.0f, 0.8660254f,
    -0.5f,  0.5f,  0.5f,  0.57735027f,  0.57735027f,  0.57735027f,  0.0f, 0.8660254f,
     0.5f, -0.5f,  0.5f,  0.57735027f,  0.57735027f,  0.57735027f,  0.5f, 0.0f,
 
    -0.5f, -0.5f, -0.5f, -0.57735027f, -0.57735027f,  0.57735027f,  0.0f, 0.1339746f,
     0.5f, -0.5f,  0.5f, -0.57735027f, -0.57735027f,  0.57735027f,  1.0f, 0.1339746f,
    -0.5f,  0.5f,  0.5f, -0.57735027f, -0.57735027f,  0.57735027f,  0.5f, 1.0f
    };
    glm::vec3 cubePositions[] = {
          glm::vec3(0.0f,  0.0f,  0.0f),
          glm::vec3(0.8f,  0.8f, -1.2f),
          glm::vec3(-0.8f, -1.2f, -0.8f),
          glm::vec3(-1.8f, 1.8f, -0.8f),
          glm::vec3(2.4f, 0.4f, -0.1f),
          glm::vec3(-1.7f, 0.6f, 0.5f),
          glm::vec3(1.3f, -2.0f, -2.5f),
          glm::vec3(1.5f,  2.0f, -2.5f),
          glm::vec3(1.5f,  0.2f, -1.5f),
          glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // 
    GLuint VBO, VAO;
    GLuint VBO2, VAO2; //Suppose I have a different shape
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 一次3个FLOAT    跨8个FLOAT再读   从0号位置开读取         
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // 一次3个FLOAT    跨8个FLAOT再读    从3个FLOAT值位置后开读
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    //glEnableVertexAttribArray(1);
    //Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // 一次2个FLOAT    跨8个FLAOT再读    从6个FLOAT值位置后开读
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO
    
    //Now for the other
    //----------------------------------------------------------------------------------------------------
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);

    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesObj), verticesObj, GL_STATIC_DRAW);

    // 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // 
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO
    //----------------------------------------------------------------------------------------------------
    
    //Configure the depth map FBO----------------------------------------------
    //const GLuint shadow_width = 2*WIDTH, shadow_height = 2*HEIGHT;
    const GLuint shadow_width = 2048, shadow_height = 2048;
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    GLuint number_of_lights = 2;
    GLuint num_trans_obj = 1;
    GLuint depthMap[number_of_lights+num_trans_obj];
    glGenTextures(number_of_lights+num_trans_obj, depthMap);
    //The full version should be +num_light*num_transobj, but I know the first light does not hit the trans onto ground or any obj
    for(int i_=0;i_<number_of_lights + num_trans_obj;++i_) 
    {
    glBindTexture(GL_TEXTURE_2D, depthMap[i_]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_width, shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);       // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //set texture WRAPPING
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor_[] = { 1.0f, 1.0f , 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor_);
    }
    //Attach the depth texture as FBO's depth buffer:
    //buffer-now-the-first-texture
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap[0], 0 );
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //FBO done ----------------------------------------------------------------

    // Load and create a texture 
    GLuint texture1;
    GLuint texture2;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // ====================
    // Texture 1
    // ====================
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on the texture object
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Load, create texture and generate mipmaps
    int width, height;
    unsigned char* image = SOIL_load_image("base.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so no mess up 
    // ===================
    // Texture 2
    // ===================
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 1.0f, 1.0f , 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    image = SOIL_load_image("chara.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);


    // ===================-----------------
    // Now have Texture 3
    // ===================-----------------
    GLuint texture3;
    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    //float borderColor[] = { 1.0f, 1.0f , 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    image = SOIL_load_image("human_not_happy.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);


    // loop
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        do_movement();

        // Render
        // Clear the colorbuffer
        glClearColor(0.5*0.2f, 0.5*0.3f, 0.5*0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Use the shadow map--------------
	glCullFace(GL_FRONT);
	//?: glUseProgram(shaderProgram_zhengchangde);
	//?: glUniform1i(glGetUniformLocation(shaderProgram_zhengchangde, "depthMap"), 3);
	glm::vec3 PointLight_position_[2];
	PointLight_position_[0]=glm::vec3(-2.5f, 2.5f, 2.5f);
        PointLight_position_[1]=glm::vec3(2.5f, 2.5f, 2.5f);
	glm::mat4 lightProj, lightView;
        glm::mat4* lightSpace = new glm::mat4[number_of_lights];
        GLfloat close_plane = 0.1f, far_plane = 100.0f;
        lightProj = glm::perspective(glm::radians(75.0f), GLfloat(shadow_width)/GLfloat(shadow_height), close_plane, far_plane);
	glUseProgram(shaderProgram_shadow);
	GLint light_space_Loc = glGetUniformLocation(shaderProgram_shadow, "lightSpace");
	glViewport(0, 0, shadow_width, shadow_height);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	GLint light_modeLoc = glGetUniformLocation(shaderProgram_shadow, "model");
	for(int i_i=0;i_i<number_of_lights+num_trans_obj;i_i++)
	{
		int i_ = i_i;
		if(i_==number_of_lights+num_trans_obj-1) i_=number_of_lights-1;
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap[i_i], 0 );
		//first light:
        //glm::mat4 lightProj, lightView, lightSpace;
	//GLfloat close_plane = 0.1f, far_plane = 100.0f;
        //lightProj = glm::perspective(glm::radians(60.0f), GLfloat(shadow_width)/GLfloat(shadow_height), close_plane, far_plane);
	//glm::vec3 PointLight_position_[0] = glm::vec3(-2.5f, 2.5f, 2.5f);
	lightView = glm::lookAt(PointLight_position_[i_], glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lightSpace[i_] = lightProj*lightView;
        //glUseProgram(shaderProgram_shadow);
        //GLint light_space_Loc = glGetUniformLocation(shaderProgram_shadow, "lightSpace");
        glUniformMatrix4fv(light_space_Loc, 1, GL_FALSE, glm::value_ptr(lightSpace[i_]));
        //glViewport(0, 0, shadow_width, shadow_height);
        //glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glActiveTexture(GL_)
	//GLint light_modeLoc = glGetUniformLocation(shaderProgram_shadow, "model");
	//--------------------------------------------------------------------------
	glBindVertexArray(VAO);
	if(i_i==number_of_lights+num_trans_obj-1)
        {
            //glBindVertexArray(VAO);
	    glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, 0.7f*PointLight_position_[i_]);
            glm::vec3 light_s_direct = glm::normalize( - PointLight_position_[i_] );
            glm::vec3 axies_ = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), light_s_direct);
            GLfloat angle = glm::acos(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), axies_));
            model = glm::rotate(model, angle, axies_);
	    model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, -1.0f) );
            model = glm::scale(model, glm::vec3(1.0f, 0.1f, 1.0f));
	    model = glm::translate(model, glm::vec3(-0.4f, 0.0, 0.4f));
            glUniformMatrix4fv(light_modeLoc, 1, GL_FALSE, glm::value_ptr(model));
            //glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
	else
	{
	    //glBindVertexArray(VAO);
            for (int i = 0; i < 2; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                GLfloat angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                glUniformMatrix4fv(light_modeLoc, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
	glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[2]);
        GLfloat angle = 20.0f * 2;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        glUniformMatrix4fv(light_modeLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 36, 12);

	glCullFace(GL_BACK);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
        model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
        glUniformMatrix4fv(light_modeLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 24, 6);
        glCullFace(GL_FRONT);

	//if(i_i==number_of_lights+num_trans_obj-1)
	//{
    	//    model = glm::mat4(1.0f);
        //    model = glm::translate(model, 0.7f*PointLight_position_[i_]);
	//    glm::vec3 light_s_direct = glm::normalize( - PointLight_position_[i_] );
	//    glm::vec3 axies_ = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), light_s_direct);
        //    angle = glm::acos(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), axies_));
        //    model = glm::rotate(model, angle, axies_);
        //    model = glm::scale(model, glm::vec3(2.0f, 0.1f, 2.0f));
        //    glUniformMatrix4fv(light_modeLoc, 1, GL_FALSE, glm::value_ptr(model));
        //    //glBindVertexArray(VAO);
        //    glDrawArrays(GL_TRIANGLES, 0, 36);
	//}

        glBindVertexArray(VAO2);
            //the sphere:
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[3]);
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(light_modeLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 3*f_num[0]);
            //draw the cone:
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[4]);
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(light_modeLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 3*f_num[0], 3*f_num[1]);
            //the cylinder:
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[5]);
            model = glm::scale(model, glm::vec3(0.25f, 0.4f, 0.25f));
            glUniformMatrix4fv(light_modeLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 3*(f_num[0]+f_num[1]), 3*f_num[2]);
	//------------------------------------------------------------------------
        }
	}	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//second light:
        //glm::vec3 PointLight_position_[1] = glm::vec3(2.5f, 2.5f, 2.5f);
        
        glCullFace(GL_BACK);
        
	// done shadow--------------------
	//Use the phone shader:
	    glViewport(0, 0, WIDTH, HEIGHT);
	    //:? glClearColor(0.5*0.2f, 0.5*0.3f, 0.5*0.3f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    glUseProgram(shaderProgram_int3);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture2);
	    glUniform1i(glGetUniformLocation(shaderProgram_int3, "material.diffuse_texture"), 0);
	    glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture3);
	    glUniform1i(glGetUniformLocation(shaderProgram_int3, "material.specular_texture"), 1);
            glUniform1f(glGetUniformLocation(shaderProgram_int3, "material.shininess"), 32.0f);
	    glUniform1f(glGetUniformLocation(shaderProgram_int3, "transparency"), 0.2f);
            glm::vec3 objs_color = glm::vec3(1.0f, 0.0f, 0.0f);
            glUniform3fv(glGetUniformLocation(shaderProgram_int3, "trans_color"), 1, glm::value_ptr(objs_color));
	    glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, depthMap[0]);
            glUniform1i(glGetUniformLocation(shaderProgram_int3, "depthMap[0]"), 2);
            light_space_Loc = glGetUniformLocation(shaderProgram_int3, "lightSpace[0]");
	    glUniformMatrix4fv(light_space_Loc, 1, GL_FALSE, glm::value_ptr(lightSpace[0]));
  	    glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, depthMap[1]);
            glUniform1i(glGetUniformLocation(shaderProgram_int3, "depthMap[1]"), 3);
            light_space_Loc = glGetUniformLocation(shaderProgram_int3, "lightSpace[1]");
            glUniformMatrix4fv(light_space_Loc, 1, GL_FALSE, glm::value_ptr(lightSpace[1]));
	    glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, depthMap[2]);
            glUniform1i(glGetUniformLocation(shaderProgram_int3, "depthMap[2]"), 4);

	    // 观察矩阵的设置
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

            GLint viewLoc = glGetUniformLocation(shaderProgram_int3, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

            // 投影矩阵的设置
            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(aspect), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
            GLint proLoc = glGetUniformLocation(shaderProgram_int3, "projection");
            glUniformMatrix4fv(proLoc, 1, GL_FALSE, glm::value_ptr(projection));
            GLint modeLoc = glGetUniformLocation(shaderProgram_int3, "model");

	    GLfloat dir_strength = 0.0f;//0.2f;
	    glm::vec3 DirLight_direction = glm::vec3(0.0f, 0.0f, 1.0f);
	    glUniform3fv(glGetUniformLocation(shaderProgram_int3, "dirLight.direction"), 1, glm::value_ptr(DirLight_direction));
	    glm::vec3 DirLight_ambient = dir_strength*0.1f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_int3, "dirLight.ambient"), 1, glm::value_ptr(DirLight_ambient));
	    glm::vec3 DirLight_diffuse = dir_strength*1.0f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_int3, "dirLight.diffuse"), 1, glm::value_ptr(DirLight_diffuse));
	    glm::vec3 DirLight_specular = dir_strength*1.9f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_int3, "dirLight.specular"), 1, glm::value_ptr(DirLight_specular));

            GLfloat co_c = 1.0f;
	    GLfloat co_1 = 0.09f;
	    GLfloat co_2 = 0.032f;

            //glm::vec3 PointLight_position_[0] = glm::vec3(-2.5f, 2.5f, 2.5f);
            glUniform3fv(glGetUniformLocation(shaderProgram_int3, "pointLights[0].position"), 1, glm::value_ptr(PointLight_position_[0]));
	    glUniform1f(glGetUniformLocation(shaderProgram_int3, "pointLights[0].constant"), co_c);
	    glUniform1f(glGetUniformLocation(shaderProgram_int3, "pointLights[0].linear"), co_1);
	    glUniform1f(glGetUniformLocation(shaderProgram_int3, "pointLights[0].quadratic"), co_2);
            glm::vec3 PointLight_ambient1 = 1.25f*0.1f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_int3, "pointLights[0].ambient"), 1, glm::value_ptr(PointLight_ambient1));
            glm::vec3 PointLight_diffuse1 = 1.25f*1.0f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_int3, "pointLights[0].diffuse"), 1, glm::value_ptr(PointLight_diffuse1));
            glm::vec3 PointLight_specular1 = 1.25f*1.9f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_int3, "pointLights[0].specular"), 1, glm::value_ptr(PointLight_specular1));

	    //glm::vec3 PointLight_position_[1] = glm::vec3(2.5f, 2.5f, 2.5f);
            glUniform3fv(glGetUniformLocation(shaderProgram_int3, "pointLights[1].position"), 1, glm::value_ptr(PointLight_position_[1]));
            glUniform1f(glGetUniformLocation(shaderProgram_int3, "pointLights[1].constant"), co_c);
            glUniform1f(glGetUniformLocation(shaderProgram_int3, "pointLights[1].linear"), co_1);
            glUniform1f(glGetUniformLocation(shaderProgram_int3, "pointLights[1].quadratic"), co_2);
            //glm::vec3 PointLight_ambient1 = 0.25f*0.1f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_int3, "pointLights[1].ambient"), 1, glm::value_ptr(PointLight_ambient1));
            //glm::vec3 PointLight_diffuse1 = 0.25f*1.0f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_int3, "pointLights[1].diffuse"), 1, glm::value_ptr(PointLight_diffuse1));
            //glm::vec3 PointLight_specular1 = 0.25f*0.9f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_int3, "pointLights[1].specular"), 1, glm::value_ptr(PointLight_specular1));

	    GLfloat spot_strength = 0.0f;//1.15f;
	    glm::vec3 SpotLight_position = glm::vec3(-2.5f, -2.5f, 2.5f);
            glUniform3fv(glGetUniformLocation(shaderProgram_int3, "spotLight.position"), 1, glm::value_ptr(SpotLight_position));
	    glm::vec3 SpotLight_direction = - glm::normalize(glm::vec3(-1.0f, -1.0f, 1.0f));
	    glUniform3fv(glGetUniformLocation(shaderProgram_int3, "spotLight.direction"), 1, glm::value_ptr(SpotLight_direction));
            glUniform1f(glGetUniformLocation(shaderProgram_int3, "spotLight.cutoff"), 0.94f);
	    glUniform1f(glGetUniformLocation(shaderProgram_int3, "spotLight.outercutoff"), 0.86f);
            glUniform1f(glGetUniformLocation(shaderProgram_int3, "spotLight.constant"), co_c);
            glUniform1f(glGetUniformLocation(shaderProgram_int3, "spotLight.linear"), co_1);
            glUniform1f(glGetUniformLocation(shaderProgram_int3, "spotLight.quadratic"), co_2);
            glm::vec3 SpotLight_ambient = spot_strength*0.1f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_int3, "spotLight.ambient"), 1, glm::value_ptr(SpotLight_ambient));
            glm::vec3 SpotLight_diffuse = spot_strength*1.0f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_int3, "spotLight.diffuse"), 1, glm::value_ptr(SpotLight_diffuse));
            glm::vec3 SpotLight_specular = spot_strength*1.9f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_int3, "spotLight.specular"), 1, glm::value_ptr(SpotLight_specular));

	    glUniform3fv(glGetUniformLocation(shaderProgram_int3, "viewPos"), 1, glm::value_ptr(cameraPos));

	    //viewLoc = glGetUniformLocation(shaderProgram_int3, "view");
            //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            //proLoc = glGetUniformLocation(shaderProgram_int3, "projection");
            //glUniformMatrix4fv(proLoc, 1, GL_FALSE, glm::value_ptr(projection));
            //modeLoc = glGetUniformLocation(shaderProgram_int3, "model");


	    // Draw container
            glBindVertexArray(VAO);
            for (int i = 0; i < 2; i++)
            {
            	glm::mat4 model = glm::mat4(1.0f);
            	model = glm::translate(model, cubePositions[i]);
            	GLfloat angle = 20.0f * i;
            	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            	glUniformMatrix4fv(modeLoc, 1, GL_FALSE, glm::value_ptr(model));

            	glDrawArrays(GL_TRIANGLES, 0, 36);
            }

	    glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[2]);
            GLfloat angle = 20.0f * 2;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            glUniformMatrix4fv(modeLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 36, 12);

            //glm::mat4 model = glm::mat4(1.0f);
            //model = glm::translate(model, cubePositions[3]);
            //angle = 20.0f * 3;
            //model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            //glUniformMatrix4fv(modeLoc, 1, GL_FALSE, glm::value_ptr(model));

            //glDrawArrays(GL_TRIANGLES, 0, 36);
	    

	    //Now I draw two point lights:-----
	    glUseProgram(shaderProgram_intL);
	    viewLoc = glGetUniformLocation(shaderProgram_intL, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	    proLoc = glGetUniformLocation(shaderProgram_intL, "projection");
            glUniformMatrix4fv(proLoc, 1, GL_FALSE, glm::value_ptr(projection));
            modeLoc = glGetUniformLocation(shaderProgram_intL, "model");

	    model = glm::mat4(1.0f);
	    //model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	    model = glm::translate(model, PointLight_position_[0]);
	    //model = glm::scale(model, 0.4f*glm::vec3(0.1f, 0.1f, 0.1f));
	    glm::vec3 light_s_direct = glm::normalize( - PointLight_position_[0] );
	    glm::vec3 axies_ = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), light_s_direct);
            angle = glm::acos(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), light_s_direct));
            model = glm::rotate(model, angle, axies_);
	    model = glm::scale(model, 0.4f*glm::vec3(0.1f, 0.1f, 0.1f));
            glUniformMatrix4fv(modeLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);

	    model = glm::mat4(1.0f);
            //model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
            model = glm::translate(model, PointLight_position_[1]);
	    //model = glm::scale(model, 0.4f*glm::vec3(0.1f, 0.1f, 0.1f));
            light_s_direct = glm::normalize( - PointLight_position_[1] );
            axies_ = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), light_s_direct);
            angle = glm::acos(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), light_s_direct));
            model = glm::rotate(model, angle, axies_);
            model = glm::scale(model, 0.4f*glm::vec3(0.1f, 0.1f, 0.1f));
            glUniformMatrix4fv(modeLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
	    //---------------------------------
            
	    //Now is no_text obj------------------------------------------------------------------------------------------
	    glUseProgram(shaderProgram_notext);
	    //glActiveTexture(GL_TEXTURE2);
            //glBindTexture(GL_TEXTURE_2D, depthMap);
            glUniform1i(glGetUniformLocation(shaderProgram_notext, "depthMap[0]"), 2);
            light_space_Loc = glGetUniformLocation(shaderProgram_notext, "lightSpace[0]");
            glUniformMatrix4fv(light_space_Loc, 1, GL_FALSE, glm::value_ptr(lightSpace[0]));
	    glUniform1i(glGetUniformLocation(shaderProgram_notext, "depthMap[1]"), 3);
            light_space_Loc = glGetUniformLocation(shaderProgram_notext, "lightSpace[1]");
            glUniformMatrix4fv(light_space_Loc, 1, GL_FALSE, glm::value_ptr(lightSpace[1]));
	    glUniform1i(glGetUniformLocation(shaderProgram_notext, "depthMap[2]"), 4);
	    //glm::vec3 objs_color = glm::vec3(255, 227, 132)/255.0f;
	    //glUniform3fv(glGetUniformLocation(shaderProgram_notext, "thecolor"), 1, glm::value_ptr(objs_color));

            viewLoc = glGetUniformLocation(shaderProgram_notext, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            proLoc = glGetUniformLocation(shaderProgram_notext, "projection");
            glUniformMatrix4fv(proLoc, 1, GL_FALSE, glm::value_ptr(projection));
            modeLoc = glGetUniformLocation(shaderProgram_notext, "model");
 	
	    glUniform1f(glGetUniformLocation(shaderProgram_notext, "material.shininess"), 32.0f);
	    glUniform1f(glGetUniformLocation(shaderProgram_notext, "trans_a"), 1.0f);
            glUniform1f(glGetUniformLocation(shaderProgram_notext, "transparency"), 0.2f);
            objs_color = glm::vec3(1.0f, 0.0f, 0.0f);
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "trans_color"), 1, glm::value_ptr(objs_color));

            //GLfloat dir_strength = 0.0f;//0.2f;
            //glm::vec3 DirLight_direction = glm::vec3(0.0f, 0.0f, 1.0f);
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "dirLight.direction"), 1, glm::value_ptr(DirLight_direction));
            //glm::vec3 DirLight_ambient = dir_strength*0.1f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "dirLight.ambient"), 1, glm::value_ptr(DirLight_ambient));
            //glm::vec3 DirLight_diffuse = dir_strength*1.0f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "dirLight.diffuse"), 1, glm::value_ptr(DirLight_diffuse));
            //glm::vec3 DirLight_specular = dir_strength*1.9f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "dirLight.specular"), 1, glm::value_ptr(DirLight_specular));

            //GLfloat co_c = 1.0f;
            //GLfloat co_1 = 0.09f;
            //GLfloat co_2 = 0.032f;

            //glm::vec3 PointLight_position_[0] = glm::vec3(-2.5f, 2.5f, 2.5f);
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "pointLights[0].position"), 1, glm::value_ptr(PointLight_position_[0]));
            glUniform1f(glGetUniformLocation(shaderProgram_notext, "pointLights[0].constant"), co_c);
            glUniform1f(glGetUniformLocation(shaderProgram_notext, "pointLights[0].linear"), co_1);
            glUniform1f(glGetUniformLocation(shaderProgram_notext, "pointLights[0].quadratic"), co_2);
            //glm::vec3 PointLight_ambient1 = 1.25f*0.1f*glm::vec3(1.0f, 0.9f, 0.8f);
            //glUniform3fv(glGetUniformLocation(shaderProgram_notext, "pointLights[0].ambient"), 1, glm::value_ptr(PointLight_ambient1));
            //glm::vec3 PointLight_diffuse1 = 1.25f*1.0f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "pointLights[0].diffuse"), 1, glm::value_ptr(PointLight_diffuse1));
            //glm::vec3 PointLight_specular1 = 1.25f*1.9f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "pointLights[0].specular"), 1, glm::value_ptr(PointLight_specular1));

            //glm::vec3 PointLight_position_[1] = glm::vec3(2.5f, 2.5f, 2.5f);
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "pointLights[1].position"), 1, glm::value_ptr(PointLight_position_[1]));
            glUniform1f(glGetUniformLocation(shaderProgram_notext, "pointLights[1].constant"), co_c);
            glUniform1f(glGetUniformLocation(shaderProgram_notext, "pointLights[1].linear"), co_1);
            glUniform1f(glGetUniformLocation(shaderProgram_notext, "pointLights[1].quadratic"), co_2);
            //glm::vec3 PointLight_ambient1 = 0.25f*0.1f*glm::vec3(1.0f, 0.9f, 0.8f);
            //glUniform3fv(glGetUniformLocation(shaderProgram_notext, "pointLights[1].ambient"), 1, glm::value_ptr(PointLight_ambient1));
            //glm::vec3 PointLight_diffuse1 = 0.25f*1.0f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "pointLights[1].diffuse"), 1, glm::value_ptr(PointLight_diffuse1));
            //glm::vec3 PointLight_specular1 = 0.25f*0.9f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "pointLights[1].specular"), 1, glm::value_ptr(PointLight_specular1));

            //GLfloat spot_strength = 0.0f;//1.15f;
            //glm::vec3 SpotLight_position = glm::vec3(-2.5f, -2.5f, 2.5f);
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "spotLight.position"), 1, glm::value_ptr(SpotLight_position));
            //glm::vec3 SpotLight_direction = - glm::normalize(glm::vec3(-1.0f, -1.0f, 1.0f));
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "spotLight.direction"), 1, glm::value_ptr(SpotLight_direction));
            glUniform1f(glGetUniformLocation(shaderProgram_notext, "spotLight.cutoff"), 0.94f);
            glUniform1f(glGetUniformLocation(shaderProgram_notext, "spotLight.outercutoff"), 0.86f);
            glUniform1f(glGetUniformLocation(shaderProgram_notext, "spotLight.constant"), co_c);
            glUniform1f(glGetUniformLocation(shaderProgram_notext, "spotLight.linear"), co_1);
            glUniform1f(glGetUniformLocation(shaderProgram_notext, "spotLight.quadratic"), co_2);
            //glm::vec3 SpotLight_ambient = spot_strength*0.1f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "spotLight.ambient"), 1, glm::value_ptr(SpotLight_ambient));
            //glm::vec3 SpotLight_diffuse = spot_strength*1.0f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "spotLight.diffuse"), 1, glm::value_ptr(SpotLight_diffuse));
            //glm::vec3 SpotLight_specular = spot_strength*1.9f*glm::vec3(1.0f, 0.9f, 0.8f);
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "spotLight.specular"), 1, glm::value_ptr(SpotLight_specular));

            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "viewPos"), 1, glm::value_ptr(cameraPos));

	    //draw the ground --------------------------------------------
	    glm::vec3 PointLight_ground_amb = 0.4f*glm::vec3(1.0f, 1.0f, 1.0f);
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "pointLights[0].ambient"), 1, glm::value_ptr(PointLight_ground_amb));
	    glUniform3fv(glGetUniformLocation(shaderProgram_notext, "pointLights[1].ambient"), 1, glm::value_ptr(PointLight_ground_amb));
	    objs_color = 1.0f*glm::vec3(255, 255, 255)/255.0f;
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "thecolor"), 1, glm::value_ptr(objs_color));
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
            model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
	    model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
            glUniformMatrix4fv(modeLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 24, 6);
            //------------------------------------------------------------

	    glUniform3fv(glGetUniformLocation(shaderProgram_notext, "pointLights[0].ambient"), 1, glm::value_ptr(PointLight_ambient1));
	    glUniform3fv(glGetUniformLocation(shaderProgram_notext, "pointLights[1].ambient"), 1, glm::value_ptr(PointLight_ambient1));
	    objs_color = glm::vec3(255, 227, 132)/255.0f;
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "thecolor"), 1, glm::value_ptr(objs_color));
            glBindVertexArray(VAO2);

            model = glm::mat4(1.0f);
            //model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
            //model = glm::translate(model, PointLight_position_[0]);
	    model = glm::translate(model, cubePositions[3]);
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            //glm::vec3 light_s_direct = glm::normalize( - PointLight_position_[0] );
            //glm::vec3 axies_ = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), light_s_direct);
            //angle = glm::acos(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), axies_));
            //model = glm::rotate(model, angle, axies_);

            glUniformMatrix4fv(modeLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 3*f_num[0]);
            
	    //draw the cone:
	    model = glm::mat4(1.0f);
            //model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
            //model = glm::translate(model, PointLight_position_[0]);
            model = glm::translate(model, cubePositions[4]);
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            //glm::vec3 light_s_direct = glm::normalize( - PointLight_position_[0] );
            //glm::vec3 axies_ = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), light_s_direct);
            //angle = glm::acos(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), axies_));
            //model = glm::rotate(model, angle, axies_);

            glUniformMatrix4fv(modeLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 3*f_num[0], 3*f_num[1]);

	    model = glm::mat4(1.0f);
            //model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
            //model = glm::translate(model, PointLight_position_[0]);
            model = glm::translate(model, cubePositions[5]);
            model = glm::scale(model, glm::vec3(0.25f, 0.4f, 0.25f));
            //glm::vec3 light_s_direct = glm::normalize( - PointLight_position_[0] );
            //glm::vec3 axies_ = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), light_s_direct);
            //angle = glm::acos(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), axies_));
            //model = glm::rotate(model, angle, axies_);

            glUniformMatrix4fv(modeLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 3*(f_num[0]+f_num[1]), 3*f_num[2]);

	    //-------xxxxxxxx---------
	    glEnable(GL_BLEND);
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	    //glDepthMask(GL_FALSE);
	    objs_color = glm::vec3(1.0f, 0.0f, 0.0f);
            glUniform3fv(glGetUniformLocation(shaderProgram_notext, "thecolor"), 1, glm::value_ptr(objs_color));
	    glUniform1f(glGetUniformLocation(shaderProgram_notext, "trans_a"), 0.2);
	    model = glm::mat4(1.0f);
            model = glm::translate(model, 0.7f*PointLight_position_[1]);
            //model = glm::scale(model, glm::vec3(1.0f, 0.2f, 1.0f));
            //light_s_direct = glm::normalize( - PointLight_position_[1] );
            //axies_ = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), light_s_direct);
            //angle = glm::acos(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), axies_));
            model = glm::rotate(model, angle, axies_);
	    model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, -1.0f) );
	    model = glm::scale(model, glm::vec3(1.0f, 0.1f, 1.0f));
	    model = glm::translate(model, glm::vec3(-0.4f, 0.0, 0.4f));
	    glUniformMatrix4fv(modeLoc, 1, GL_FALSE, glm::value_ptr(model));
	    glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
	    glDisable(GL_BLEND);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            //glDepthMask(GL_TRUE);
	    //-------xxxxxxxx---------
            //Now end the no_text obj------------------------------------------------------------------------------------------
	    //draw the ground --------------------------------------------
	    //objs_color = glm::vec3(112, 128, 105)/255.0f;
            //glUniform3fv(glGetUniformLocation(shaderProgram_notext, "thecolor"), 1, glm::value_ptr(objs_color));
	    //model = glm::mat4(1.0f);
            //model = glm::translate(model, glm::vec3(0.0f, -4.0f, 0.0f));
            //model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
	    //glUniformMatrix4fv(modeLoc, 1, GL_FALSE, glm::value_ptr(model));
            //glDrawArrays(GL_TRIANGLES, 3*(f_num[0]+f_num[1]), 3*f_num[2]);
	    //------------------------------------------------------------

        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeletProgram(shaderProgram_int/*(& | 2 ?)//);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        mixValue += 0.1f;
        if (mixValue >= 1.0f)
            mixValue = 1.0f;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        mixValue -= 0.1f;
        if (mixValue <= 0.0f)
            mixValue = 0.0f;
    }
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.1;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}
void do_movement()
{
    // 相机控制
    GLfloat cameraSpeed = 5.0f * deltaTime;
    if (keys[GLFW_KEY_W])
        cameraPos += cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_S])
        cameraPos -= cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (aspect >= 1.0f && aspect <= 45.0f)
        aspect -= yoffset;
    if (aspect <= 1.0f)
        aspect = 1.0f;
    if (aspect >= 45.0f)
        aspect = 45.0f;
}


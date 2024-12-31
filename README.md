# 图形引擎

GitHub：https://github.com/buyicoder/GraphEngine.git

Contributors:buyicoder、wolflemon、huyuxiu7

## 我们的工作

我们在实时三维图形基础的第三次作业的代码基础上实现了一个基本的图形渲染引擎框架。

### 目前的核心功能

在世界场景中加载模型，从不同视角，不同光照下观察模型，编辑模型。

### 我们的工作如下：

- 完善光栅化渲染管线
- 交互模式管理：暂分为场景游览模式，全局编辑模式，文字编辑模式，不同交互模式下，程序对鼠标键盘的响应不同，按ESC进入全局编辑模式，显示鼠标光标，鼠标点击场景区域进入场景游览模式，可操控相机移动在场景中游览，打开文本文件进入文本编辑模式，可进行文字编辑

- 增加UI界面

  - 菜单栏：文件，编辑，视图，Git，项目等等，鼠标悬浮可显示相应选项
  - 窗口栏：显示已经打开的窗口，可以拖动互换位置，可隐藏

- 实现文件管理器

  - 可在程序内浏览真实文件
  - 创建文件夹，自由命名
  - 创建文件，自由命名

- 增加模型管理器

  - 快速切换观察视角（前后左右）
  - 控制模型自动旋转
  - 网格简化功能
  - 加载更多模型
  - 后续会增加鼠标控制缩放，变形，移动等功能

- 可扩展性代码重整，实时系统分为

  - 相机管理系统：管理场景中的相机，初始化出主相机（可移动），和固定视角相机（前后左右）
  - 文件管理系统：实现创建文件（夹），修改文件，保存文件，
  - 输入管理系统：存储输入状态处理鼠标，键盘等输入设备的响应事件
  - 图形管理系统：加载字体和屏幕设置
  - 交互管理系统：处理所有的窗口渲染和鼠标点击逻辑
  - 窗口管理系统：管理所有窗口的激活和打开状态
  - 模型管理系统：管理所有场景内加载的模型
  - 场景管理系统：管理场景内所有对象：包括模型，辅助网格，等等
  - 光照管理系统：管理场景内光照

  便于后续扩展各个模块的功能，交互管理系统实现前后端分离开发。

## 一、基本架构

### （一）核心模块

#### 1. 图形渲染基础模块

负责图形渲染的基础设置和操作，包括 OpenGL 环境的初始化、窗口创建和管理、着色器的使用等。

包含文件：

glad.h：用于加载OpenGL函数指针，确保可以使用OpenGL的各种函数。

glfw3.h：创建和管理窗口、处理用户输入、管理 OpenGL 上下文等。

glm.hpp、matrix_transform.hpp、type_ptr.hpp：提供数学运算和矩阵变换功能，如生成投影矩阵、旋转矩阵等。

Shader.h：着色器类，用于创建、编译、使用和管理着色器程序，控制图形的渲染效果。

 

#### 2. 用户界面模块

使用 Dear ImGui 库创建用户界面，提供用户交互和信息显示的功能。

包含文件：

imgui.h ,imgui_impl_glfw.h , imgui_impl_opengl3.h：用于创建和管理用户界面元素，如按钮、滑块、菜单等，将用户界面集成到 OpenGL 程序中，方便用户进行交互操作。

 

#### 3. 网格处理模块

处理网格数据，包括网格的简化和其他操作。

mesh_simplification.h：包含网格简化的算法和相关操作，用于优化模型的网格，减少模型的复杂度，提高渲染性能。

 

### （二）辅助模块

#### 1. 系统模块

将不同的系统功能进行了模块化封装，方便管理和维护程序的不同部分。

包含文件：

WindowSystem.h：用于处理窗口相关的操作，如窗口的创建、大小调整、关闭等。

ModelSystem.h：对模型相关操作的封装，包括模型的加载、更新、渲染等。

GraphicsSystem.h：涉及图形渲染的核心功能，如绘制图形、设置渲染状态等。

InputSystem.h：处理用户输入，如键盘、鼠标输入，包括事件处理和输入回调函数的注册等。

CameraSystem.h：专门负责相机系统的操作，如相机的移动、旋转、缩放等操作。

InteractSystem.h：处理用户与程序之间的交互操作，将用户输入转化为程序的相应动作。

InterModeSystem.h：涉及程序不同交互模式的管理，根据不同的交互模式执行不同的操作。

LightSystem.h：用于管理光照，包括光源的位置、强度、颜色等参数的设置。

SceneManageSystem.h：负责管理场景中的元素，如添加、删除、更新场景中的对象。

 

#### 2. 自定义工具和辅助模块

一系列工具和辅助类，以支持程序的各种功能。

包含文件：

tools.h：包含各种工具函数，用于实现一些通用的辅助功能。

shader.h：着色器类，封装了一些着色器相关的操作，如加载着色器源代码、编译着色器、设置着色器参数等。

camera.h：相机类，负责管理相机的位置、视角、投影矩阵等，为渲染提供视角信息。

model.h：模型类，用于加载和管理 3D 模型，处理模型数据。

 

### （三）架构关系

#### 1. 模块之间依赖关系

基础库依赖：

程序整体依赖 GLFW 进行窗口和上下文的创建和管理，以及用户输入的处理。

glad 用于加载 OpenGL 函数指针，使程序能够调用 OpenGL 的各种函数，因此它是使用 OpenGL 进行图形渲染的基础。

glm 提供了矩阵运算和几何变换的支持，是实现 3D 图形变换的基础数学库，为 Shader 和 Camera 等模块提供数学计算支持。

imgui 及其相关的后端实现 imgui_impl_glfw 和 imgui_impl_opengl3 用于创建用户界面，依赖 GLFW 和 OpenGL 提供的环境进行界面的显示和交互。

 

自定义模块间的协作：

WindowSystem 是对 GLFW 窗口管理功能的封装，为整个程序提供窗口创建和管理的功能，其他模块间接依赖它来进行图形的展示。

CameraSystem 与 Camera 类紧密相关，它管理 Camera 对象的创建、更新和使用，为 Shader 模块提供投影矩阵和视图矩阵信息，用于在渲染时进行视角转换和投影变换。

Shader 类在整个渲染过程中起着核心作用，它接收来自 CameraSystem 的矩阵信息以及来自 LightSystem 的光照信息，同时为 ModelSystem 中的 Model 对象提供渲染时的着色器程序，以控制模型的外观。

ModelSystem 依赖 Shader 进行渲染，它管理多个 Model 对象的加载、更新和绘制，通过调用 Shader 的相关接口设置模型的渲染属性，并使用 GraphicsSystem 中的相关函数进行实际的绘制操作。

GraphicsSystem 包含一些底层的图形绘制操作，如 glDrawArrays、glBindVertexArray 等，为 ModelSystem 和 DrawAxis 等函数提供图形绘制的底层支持。

InputSystem 通过 GLFW 注册的回调函数接收用户输入，并将这些输入信息传递给 InteractSystem 进行处理，InteractSystem 会根据输入信息触发不同的操作，如修改 Camera 的位置、选择或操作 Model 等。

InterModeSystem 影响程序的交互模式，不同的交互模式会影响 InputSystem 和 InteractSystem 的行为，例如在不同模式下，鼠标点击或键盘输入会触发不同的操作。

LightSystem 负责管理光照信息，它将光照参数传递给 Shader 类，影响渲染结果，使场景中的模型具有光照效果。

SceneManageSystem 负责管理整个场景，它协调 ModelSystem、LightSystem 和 CameraSystem 等模块，确保场景中的对象、光照和视角协调工作。

 

#### 2. 数据流向

用户输入数据：

首先通过 GLFW 的回调函数被 InputSystem 捕获，InputSystem 将输入信息传递给 InteractSystem。

InteractSystem 根据当前的交互模式将输入信息转换为具体的操作，例如修改 Camera 的位置或状态，或者对 Model 进行操作。

 

图形渲染数据：

Model 对象的数据从文件中加载到 ModelSystem，ModelSystem 将 Model 的顶点数据存储在 VBO 和 VAO 中，使用 GraphicsSystem 的底层 OpenGL 函数进行绘制。

Shader 程序根据 Camera 提供的投影矩阵和视图矩阵以及 LightSystem 提供的光照信息对 Model 进行着色，最终通过 GraphicsSystem 的 glDrawArrays 等函数将模型绘制到屏幕上。

 

## 二、API接口说明

#### （一）OpenGL API接口

glfwInit()：初始化 GLFW 库，为创建窗口和管理 OpenGL 上下文做准备。

glfwWindowHint()：设置 GLFW 窗口的各种属性，如 OpenGL 版本、使用的配置文件等。

glfwCreateWindow()：创建一个 GLFW 窗口对象，指定窗口的大小、标题等信息。

glfwSetInputMode()：设置窗口的输入模式，如鼠标是否可见等。

glfwSetCursorPosCallback()、glfwSetScrollCallback() 等：设置各种用户输入的回调函数，用于处理鼠标和键盘事件。

glfwGetProcAddress()：获取 OpenGL 函数的地址，配合 GLAD 库使用，以加载 OpenGL 函数指针。

gladLoadGLLoader()：加载 OpenGL 函数指针，使程序可以使用 OpenGL 的各种函数。

glGenVertexArrays()、glGenBuffers()：生成顶点数组对象（VAO）和顶点缓冲对象（VBO），是 OpenGL 中管理顶点数据的基本操作。

glBindVertexArray()、glBindBuffer()：绑定 VAO 和 VBO 到当前上下文，以便进行后续操作。

glBufferData()：将数据存储到 VBO 中，可用于存储顶点数据、索引数据等。

glVertexAttribPointer()、glEnableVertexAttribArray()：设置顶点属性指针，告诉 OpenGL 如何解析顶点数据。

glDrawArrays()、glDrawElements()：执行绘制操作，根据存储在 VAO 和 VBO 中的数据绘制图形。

glClear()、glClearColor()：清除颜色缓冲和深度缓冲，设置清除颜色。

glEnable()：启用各种 OpenGL 状态，如 GL_CULL_FACE 用于背面剔除，GL_DEPTH_TEST 用于深度测试。

 

#### （二）Dear ImGui API接口

ImGui::CreateContext()：创建 Dear ImGui 的上下文。

ImGui::StyleColorsLight()：设置 Dear ImGui 的颜色风格。

ImGui_ImplGlfw_InitForOpenGL()、ImGui_ImplOpenGL3_Init()：初始化 Dear ImGui 库，使其与 GLFW 和 OpenGL 集成。

ImGui::NewFrame()、ImGui::Render()、ImGui_ImplOpenGL3_RenderDrawData()：用于更新和渲染 Dear ImGui 的用户界面。

 

#### （三）自定义接口

Shader 类的接口：

Shader(shader_path_vert, shader_path_frag)：Shader 类的构造函数，用于创建和编译着色器程序，接收顶点着色器和片元着色器的文件路径。

use_program()：用于激活着色器程序的方法，使后续的绘制操作使用该着色器。

set_vecf3()、set_float()、set_matf4()：用于设置着色器中的各种变量，如向量、浮点数、矩阵等。

delete_program()：用于删除着色器程序，释放资源。

Camera 类的接口：

get_projection_matrix()、get_view_matrix()：用于获取相机的投影矩阵和视图矩阵，用于渲染时的矩阵变换。

Model 类的接口：

包含模型的加载、更新、渲染等操作，例如 LoadModel()、DrawModel() 等。

process_input()：用于处理用户输入，如键盘和鼠标输入。

DrawEditor()：用于绘制用户界面，使用了 Dear ImGui 库。

GenerateGridVertices()：用于生成网格顶点数据。

 

## 三、应用Demo展示

### （一）基本功能

窗口和用户界面：

程序启动时，通过 GLFW 创建一个窗口，并使用 imgui 实现用户界面，用户可以在窗口中看到图形界面元素，如按钮、滑块等。例如，可以通过用户界面调整光照强度、颜色等参数。

3D 场景渲染：

显示一个 3D 场景，其中包含一个网格（通过 GenerateGridVertices 生成），多个模型（通过 load_meshes 加载），以及坐标轴（通过 InitAxis 初始化并使用 DrawAxis 绘制）。

用户交互：

用户可以使用鼠标和键盘进行交互，由于 glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); ，鼠标光标被隐藏，会使用鼠标移动控制 Camera 的视角。

例如，通过 process_input 函数处理用户输入，用户可以通过鼠标和键盘的操作来移动 Camera 或操作场景中的 Model 对象。

光照和材质效果：

使用 Shader 类设置光照信息，如点光源的位置、辐射强度、环境光等。在 main 函数中，对 shader 进行设置，将光照参数传递给着色器程序，使模型具有光照效果，并且可以在 Shader 类中设置粗糙度、金属度等材质参数，从而改变模型的外观。

 

以下是一个简单的应用 Demo 展示流程：

启动程序：

调用 glfwInit 初始化 GLFW 库，创建窗口并设置上下文，使用 gladLoadGLLoader 加载 OpenGL 函数指针。初始化 imgui 并设置相关的 OpenGL 和 GLFW 后端，同时加载支持中文的字体。

场景和对象初始化：

加载 Shader 程序，设置光照和材质参数。加载模型和网格数据，创建 VBO 和 VAO 存储顶点数据，如网格顶点通过 GenerateGridVertices 生成并存储。初始化坐标轴，通过 InitAxis 函数生成 axisVAO 和 axisVBO 存储坐标轴顶点信息。

渲染循环：

在 while (!glfwWindowShouldClose(window)) 循环中：首先，使用 RecordTime 记录时间，用于计算帧率或动画等。通过 SetInterMode 设置交互模式，根据 is_changing 的状态处理输入或更新模型索引。更新 imgui 的帧信息（ImGui_ImplOpenGL3_NewFrame、ImGui_ImplGlfw_NewFrame、ImGui::NewFrame）。绘制用户界面，包含各种操作按钮和参数设置界面（DrawEditor）。清除颜色和深度缓冲，设置清除颜色（glClearColor、glClear）。设置 Shader 的参数，包括相机位置、投影矩阵、视图矩阵等，将 Shader 应用到网格和模型上进行渲染。绘制网格（使用 glBindVertexArray(VAO) 和 glDrawArrays）。绘制模型（通过 DrawModel 函数），使用 Shader 对模型进行着色和渲染。绘制坐标轴（通过 DrawAxis 函数），使用 axisShader 进行着色。最后，使用 ImGui 渲染用户界面元素（ImGui::Render、ImGui_ImplOpenGL3_RenderDrawData），并交换前后缓冲区（glfwSwapBuffers）和处理事件（glfwPollEvents）。

 

### （二）功能扩展

动态更新：

通过 is_changing 变量和 *getCurrentIndex() 的使用，程序支持动态更新显示的模型，例如在一个模型集合中切换显示不同的模型。

交互模式切换：

SetInterMode 函数允许用户在不同的交互模式下切换，例如在观察模式下可以移动相机，在编辑模式下可以对模型进行操作，不同模式下用户输入的响应会有所不同。

## 四、后续扩展方向

### 1.高级图形特性支持：

引入高级光照模型，如PBRF等；支持体积渲染技术，用于模拟烟雾、雾和云等效果；实现环境光遮蔽，以增强场景的真实感。

### 2.性能优化：

实现更高效的资源管理，包括内存管理和GPU资源的优化；支持图形管线的异步计算以提高资源利用率。

### 3.用户界面增强：

集成更丰富的用户界面元素和控件，提供更直观的参数调整和场景管理；

支持用户界面的自定义和布局保存，以便用户可以根据个人喜好进行设置。

### 4.物理模拟：

集成物理引擎，支持刚体和软体的物理模拟，如碰撞检测、动力学模拟等；

实现流体动力学模拟，用于模拟水、火、烟等流体效果。

### 5.多人交互：

支持网络多人在线协作和交互，允许多个用户在同一虚拟环境中进行操作。

### 6.扩展性增强：

提供插件系统，允许开发者根据需要添加新的功能模块；支持脚本语言，允许用户通过编写脚本扩展引擎功能或自定义行为。
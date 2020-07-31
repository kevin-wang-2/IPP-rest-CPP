##一个纯C++的RESTful API框架

这个框架为交大密院"无人机物流"IPP项目提供RESTful API服务器支持。非通用框架，因此不接受外来PR。

# 简介

* 该框架使用原生API实现Socket服务器，采用消息循环调度模式
* 框架通过Parser解析HTTP请求，通过Router中间件生成响应，通过扩展可以获得更复杂的解析

# 编译

* UNIX下cmake + g++
* WINDOWS下MINGW32 POSIX线程

# 使用

### Router及路由中间件

* Router为单例类，可以通过Router::getRouter()函数来获取可用的Router实例
* Router.use() 函数接收RouterCallable实例或智能指针以加载自定义路由中间件
* Router会链式调用中间件直到中间件表明请求结束(run函数中不调用next函数)。
* 重载RouterCallable虚基类并重载run函数以完成中间件的作用;重载getIdentity函数以获得唯一标识符

### 函数路由中间件

* FunctionalRouterCallable为基本的函数中间件提供支持，若函数返回true视为请求已结束。
* 若中间件无记忆属性，可直接使用FunctionalRouterCallable并传入函数指针/函数对象/lambda函数来作为中间件处理器

### HTML路由中间件

* HTMLRouter只提供针对指定路径或者其下子路径提供路由，并会将子路径转换为URL参数以便RESTful API使用。

### 静态路由中间件

* 静态路由中间件独立于其它中间件，因为不需要提供处理函数。它会直接反应本地静态路径至目标路径，并自动设置MIME类型。

# Logger

* 该服务器使用自己编写的简单(非常蠢的)Logger，若有需要可以更换成商业级Logger。

# 依赖

* 服务器本体目前没有依赖项，若需要处理RESTful API中的JSON，可以使用附带的RapidJSON库(tx爸爸nb)。
##### https://github.com/Caesarhhh/Pop-up-prevention


# software test-Pop up prevention
 ##### 软件测试课做的一个防弹窗作业

### 按键说明

##### AUTO:ON     设置开机自启动

##### AUTO:OFF    关闭开机自启动

##### AddWeb：将输入框中的信息加入到WebNames

##### AddWin： 将输入框中的信息加入到Banlists

##### SetHook： 设置监视应用程序弹窗的钩子

##### EndHook：关闭监视应用程序弹窗的钩子

##### SetWebHook： 设置监视网页弹窗的钩子

##### EndWebHook： 关闭监视网页弹窗的钩子

### 备注

##### 1.目前监视网页弹窗以及应用程序弹窗依据的是弹窗类名以及标题，所以Banlists和WebNames中的信息可以是类名关键词也可以是标题关键词（类名可以用visual studio中的spy++工具，以后有时间会在本项目中加入类似的功能）

##### 2.目前没有将32位弹窗和64位弹窗的钩子综合起来所以做了两份exe，以后有时间再合成一个 QAQ

##### 3.网页弹窗检测做的只是单独弹窗的（新建窗口打开的那种），没有做标签页的，以后有时间更新QAQ

##### 4.需要修改banlist以及webnames的可以直接修改程序所在文件夹下的文件

##### 5.可以在explorelist.txt中添加需要监测的浏览器的类名







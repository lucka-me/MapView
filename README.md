<h1 align=center><img src="./Resource/Banner.png" alt="DeMap"></h1>

<p align="center">
  <a href="https://github.com/lucka-me/MapView/commits/master"><img alt="Version" src="https://img.shields.io/badge/version-1.0-brightgreen.svg"/></a>
  <a href="https://lucka.moe"><img alt="Author" src="https://img.shields.io/badge/author-Lucka-5880C8.svg"/></a>
  <a href="./LICENSE"><img alt="License" src="https://img.shields.io/badge/license-MIT-A31F34.svg"/></a><br>
  <a href="https://www.visualstudio.com/zh-hans/vs/community/"><img alt="IDE" src="https://img.shields.io/badge/IDE-Visual_Studio_Community_2017-8662C2.svg"/></a>
  <a href="https://www.microsoft.com/zh-cn/windows/"><img alt="Platform" src="https://img.shields.io/badge/platform-Windows-137AD4.svg"/></a>

</p>
<p align="center">
  基于微软基础类库（<b>M</b>icrosoft <b>F</b>oundation <b>C</b>lasses）开发的地图显示软件<br>
  计算机地图制图实习作业
</p>

## Description
读取地图数据，进行坐标变换、建立格网索引并显示地图，提供基本的缩放、移动、点击查询和按分类编号查询功能。 在 [DrawDemo](https://github.com/lucka-me/DrawDemo) 的基础上开发，复用了部分数据结构和方法并进行了一些优化和调整。

### Fnctions
* **读取数据**
  读取 `.usr` 文件中的数据并生成一个要素列表。
* **坐标变换**
  输入三个控制点的规定坐标，对全部要素的坐标进行仿射变换。
* **显示地图**
  将地图显示在文档视图中，每类要素（如河流湖泊、居民地）以不同的样式显示。
* **建立索引和查询**
  按指定的分辨率建立规则格网索引，在一个二维数组中存储各个格网中涉及的要素列表。用户可在视图中点击查询要素。

## Requirements
### Environment
本程序在 Visual Studio Community 2017 上开发，在 Windows 10 上测试运行正常。

### File
| Filename | Description
| :------- | :---
| `*.usr`  | 地图数据文件，详细格式见下

数据分为点、线（折线）和面（多边形）三种，文件中每行为一对数字，具体格式如下：

```
...
序号, 0
分类编码, 0
[坐标对列表]
-666666.0,-666666.0
...
-999999.0,-999999.0
```
其中 `-666666.0,-666666.0` 标志坐标对列表结束，`-999999.0,-999999.0` 标志文件结束。

支持的分类代码如下：
| Code | Type | Name
| :--- | :--- | :---
| `30000` | 点 | 控制点
| `10000` | 点 | 图廓点
| `10001` | 线 | 铁路
| `10003` | 线 | 汽渡虚线
| `10004` | 线 | 主要道路
| `20001` | 面 | 码头
| `20002` | 面 | 铁路中转站
| `20003` | 面 | 河流、湖泊
| `20004` | 面 | 居民地


## License
本软件基于 [MIT 协议](./LICENSE)。

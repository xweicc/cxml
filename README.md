# C语言写的xml解析器
### API说明
- `cXmlParse` 解析XML文本，成功返回cXml结构，失败返回NULL
- `cXmlDelete` 释放cXml结构
- `cXmlPrint` 打印cXml结构
- `cXmlGetItem` 通过name获取cXml结构中的子节点
- `cXmlGetNextSameItem` 取与当前name相同的下一个节点，用于遍历数组
- `cXmlGetAttr` 通过name获取cXml节点的属性
### 使用示例
- 编译：`gcc demo.c cxml.c -o demo`
- 运行：`./demo test.xml`
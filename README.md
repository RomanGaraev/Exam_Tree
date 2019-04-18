# Exam_Tree
<h4>Описание программы</h4>
<h5>В этой программе реализованна функция поиска наименьшего общего предка в дереве. Дерево n-ое, т.е. каждый узел
имеет случайное число потомков(2<=n<=9). Параметры дерева, глубина и вероятность порождения следующего поколения, 
берутся из .xml-файла.
Во время построения дерева в консоль выводится инфа о каждом узле (уровень, вид, имя). Листовые узлы (без потомков)
названы "терминальными", внутренние узлы никак не названы. "Достроенными" называются узлы, которые программа достроила
сама. Такое происходит, если вероятность из файла слишком мала. Корень дерева не имеет уникального номера и назван "root"</h5>
<h4>Как работать</h4>
<h5>1) Укажите имя файла (в проекте это Test.xml)</h5>
<h5>2) Дождитесь завершения построения дерева</h5>
<h5>3) Введите имена узлов, предка которых Вы хотите найти. Например, кусок дерева:</h5>
<h5>"...</h5>
<h5>Level 3,node (terminal)  605</h5>
<h5>Level 3,node (terminal)  606</h5>
<h5>..."</h5>
<h5>Вводим имена:</h5>
<h5>"Enter node names: 605 606"</h5>
<h5>Вывод:</h5>
<h5>"Ancient name: 60"</h5>

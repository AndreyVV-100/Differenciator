# Differentator

Эта программа берёт частную производную от произвольного выражения. Для этого считывается арифметическое выражение методом рекурсивного спуска (см. репозиторий ```Calculator```). Затем выражение обрабатывается в два этапа:

1. Взятие производной (см. функцию ```DiffFunction```). Оно производится по правилам математического анализа. Стоит отметить, пожалуй, единственную особенность этой части: при дифференцировании степенной функции рассматриваются различные случаи: степенная, показательная и произвольная функции.

2. Упрощение выражения (см. функцию ```Simplifier```). В нём выполняется свёртка констант (дерево ```1 + 2``` преобразуется в ```3``` и т.д.) и удаление нейтральных элементов (дерево ```1 * x``` преобразуется в ```x``` и т.д.). Стоит отметить, что бинарность дерева значительно усложняет работу упростителя, поэтому он далеко не всегда срабатывают эти действия, а более сложные выражения (например, ```x + x```) он не рассматривает.

## Спасибо за внимание!

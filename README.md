# schem

– аппаратно-программное средство для изучения цифровых микросхем малой степени интеграции в корпусах типа DIP.

![](https://github.com/he11ca7/schem/blob/master/pic/ShieldWDuino.JPG)

Концептуально средство состоит из четырёх частей:
- Аппаратная часть в формате Arduino Shield.
- Произвольная изучаемая микросхема (далее МС).
- Программная часть в составе ПК.
- Программная часть в составе Arduino.

Управление изучаемой МС и контроль её состояния производится посредством GUI программы части ПК.

## Аппаратная часть

![](https://github.com/he11ca7/schem/blob/master/pic/Shield.JPG)

Аппаратная часть представляет из себя плату в формате Arduino Shield.
Выводы ZIF-панели продублированы с обеих сторон платы-расширения. Это позволяет легко конфигурировать плату-расширение под любую расстановку контактов МС типов корпусов DIP14 и DIP16 посредством установки перемычек в контактные колодки. На данной схеме плата-расширение сконфигурирована под микросхемы в корпусе DIP16 с характерным для серии 1533 расположением выводов питания.

## Установка

Программные составляющие средства со стороны ПК совместимы с системами, поддерживающими Qt5; со стороны Arduino -- с Arduino-совместимыми платформами.

Для установки требуется Arduino IDE и Qt IDE (Qt5):
1) Прошить Arduino – **iskra/iskra.ino**.
2) Собрать программу части ПК – **schem.pro**.

## Конфигурация выводов

Физическая конфигурация выводов задаётся в iskra/iskra.cpp. К примеру, для конфигурации, изображённой выше:

```cpp
const int Iskra::_config[] =
{
    13, 12, 11, 10, 9, 8, 7, A0,
    0, 1, 2, 3, 4, 5, 6, A0
};
```

Контакты питания, земли и неиспользуемые обозначаются любым свободным контактом.

## Формат описания МС и УГО

Во момент запуска ПО части ПК производится загрузка файлов описания МС и УГО (условное графическое обозначение) в файлах формата **chip** и **ugo** соответственно.

Пример файла описания МС:

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<chip>
  <name>КР1533ИЕ7</name>
  <description>ЧЕТЫРЕХРАЗРЯДНЫЙ ДВОИЧНЫЙ РЕВЕРСИВНЫЙ СЧЕТЧИК</description>
  
  <pin id="00" state="STATE_IN">
      <name>D2</name>
      <description>Вход предзаписи 2</description>
  </pin>
  <pin id="01" state="STATE_OUT">
      <name>Q2</name>
      <description>Выход разряда 2</description>
  </pin>
  ...
  <pin id="07" state="STATE_GND">
      <name>0V</name>
      <description>Общий вывод</description>
  </pin>
</chip>
```

Пример файла описания УГО:

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<ugo>
  <name>КР1533ИЕ7</name>
  <pins align="left">4, 3, 14, 0, 9, 8, 10, 13</pins>
  <pins align="right">11, 12, 2, 1, 5, 6</pins>
  <limiters align="left">1, 2, 7</limiters>
  <limiters align="right">2</limiters>
</ugo>
```

Файл описания УГО представляет из себя описание расположения выводов МС соответственно техническому описанию. При этом имеется возможность установки разделителей (горизонтальных разделительных линий) посредством тега **limiters**.

## Прошивка

Перед началом управления выполняется соединение с подключенной к ПК Arduino и выбор описания МС.

![](https://github.com/he11ca7/schem/blob/master/pic/Patch.gif)

## Управление

Управление МС может осуществляться как посредством интерфейса МС, так и посредством интерфейса УГО. Данные интерфейсы работают синхронно.

![](https://github.com/he11ca7/schem/blob/master/pic/Control.gif)

## Ограничения

На изучаемые МС накладываются ограничения:
- Корпуса типов DIP14 и DIP16.
- Статический характер работы МС: на каждое входное воздействие МС должна реагировать однократным изменением состояния выводов. К таким МС относятся, к примеру, ЛА2/3/4/6, ИЕ6/7, ИП7.

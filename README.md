Скетч для Arduino Mega на чипе ATMega 2560.
Требуется для устройства созданного для задания и отображения требуемых координат
на модернизированном токарном станке под сварочный аппарат.
Для удобства сварщика устройство отображает две точки для места варки сварочным аппаратом.
На пяти 7-сегментных 4-х разрядных дисплех.
Точку Х1, Х2, а так же угол для каждой точки, У1 и У2. Дополнительно отображается выбраная позиция.
Всего позиций 25 шт. 
Для станка был изготовлен блок с 25 концевыми наконечниками, при заажатии которого осуществляется опредление позиции.
Позиция высчитывается через блок резисторов - энкодер, который имеет подстроечный резистор.
Подстройка осуществляется по 20-25 позиции, при подключении компьютера к ардуино.
Данные координат задаются в csv файле на microSD карте, затем считываются, преобразовываются в верный формат для arduino и
передаются на дисплеи.

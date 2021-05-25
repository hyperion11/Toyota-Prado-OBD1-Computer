#ifndef EXTRA_H
#define EXTRA_H



char * TimeToString(uint32_t t)
{
  static char str[8];
  if (t <= 356400000)  //если счетчик часов меньше 99 то выводится время с секундами
    sprintf(str, "%02d:%02d:%02d", int(t / 3600000) , int((t / 60000) % 60), int(t / 1000) % 60);
  else  //в противном случае если больше 99 то выводится без секунд, чтобы не налазило на след текст
    sprintf(str, "%02d:%02d", int(t / 3600000) , int((t / 60000) % 60));
  return str;
}

uint16_t GetKeyValue(uint8_t PIN) {         // Функция устраняющая дребезг
  static uint8_t   count;
  static uint16_t   oldKeyValue; // Переменная для хранения предыдущего значения состояния кнопок
  static uint16_t   innerKeyValue;
  uint16_t actualKeyValue = analogRead(PIN);  // Получаем актуальное состояние
  actualKeyValue = ((actualKeyValue - 128) >> 10) + 1;   // Для 4 кнопок
  if (innerKeyValue != actualKeyValue) {  // Пришло значение отличное от предыдущего
    count = 0;                            // Все обнуляем и начинаем считать заново
    innerKeyValue = actualKeyValue;       // Запоминаем новое значение
    //Serial.println(innerKeyValue);
  }
  else {
    count += 1;                           // Увеличиваем счетчик
  }
  if ((count >= 10) && (actualKeyValue != oldKeyValue)) { // Счетчик преодолел барьер, можно иницировать смену состояний
    oldKeyValue = actualKeyValue;         // Присваиваем новое значение
  }
  return    oldKeyValue;
}


uint8_t str2month(const char * d)
{
  uint8_t i = 13;
  while ( (--i) && strcmp(months[i], d) != 0 );
  return i;
}
#endif

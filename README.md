# Discrete Analysis coursework - Spell Checker

Используется алгоритм Noisy Channel. В качестве модели ошибки берутся все слова с расстоянием Дамерау-Левенштейна = 2,
языковая же модель это Knesser-Ney Smoothing, натренированный на 200 метровом сборнике статей из англоязычных блогов на ЖЖ.
Модель у меня 3 словная и 19 вариантная (20-е - само слово, моделирование варианта, когда словарное слово на самом не ошибочно).

Перед использованием нужно распаковать текстовый файл с языковой моделью из архива. В процессе использования создает в начале рабочий файл "fdict_bin" для ускоренной загрузки в дальнейшем.

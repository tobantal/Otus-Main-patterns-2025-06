# Обзор домашних работ по курсу Архитектура и шаблоны проектирования

https://docs.google.com/document/d/1EljPK064wakuslgQpxIk5eZ-ojkAvoPSAoKvra4O2v0/edit?pli=1&tab=t.0#heading=h.9b7q2za2plfw

## ДЗ 1 Разработать набор модульных тестов

### Что такое NaN?

`NaN` — это значение, возвращаемое определенными математическими операциями, когда их результат не определен или не представляет собой действительное число в рамках стандарта IEEE 754 для арифметики с плавающей запятой.

double result \= 0.0 / 0.0; // result \= NaN

double result \= double.PositiveInfinity / double.PositiveInfinity; // result \= NaN

double result \= 0.0 \* double.PositiveInfinity; // result \= NaN

double result \= Math.Sqrt(-1.0); // result \= NaN

double nan \= double.NaN;

double result \= nan \+ 10; // result \= NaN

result \= nan \* 5;         // result \= NaN

Из-за свойства неравенства самому себе нельзя использовать обычное сравнение (==).

Как проверить?

if (double.IsNaN(value)) {

    // Обработка случая, когда результат не число

}

if (value \!= value)  {

    // Это условие выполнится ТОЛЬКО если value — NaN.
    
    // Для любого другого числа, даже Infinity, value \== value вернет true.

}

Пример из ДЗ:

// Где-то произошла ошибка

Vector2 invalidDirection \= new Vector2(double.NaN, 1); 

// Движение будет рассчитано некорректно

Vector2 newPosition \= oldPosition \+ invalidDirection \* speed; 

// newPosition теперь тоже содержит NaN

// При следующем рендеринге объект с координатами (NaN, NaN) просто не будет отрисован на экране.

// Это лучше, чем падение всей игры, но obviously является багом, который нужно искать.

## ДЗ 2 Движение объектов

1\. ДЗ сдано на проверку \- 1 балл  
2\. Оформлен pull/merge request на github/gitlab \- 1 балл  
3\. Настроен CI \- 2 балла  
4\. Прямолинейное равномерное движение без деформации.  
4.1. Само движение реализовано в виде отдельного класса \- 1 балл.  
4.2. Для движущихся объектов определен интерфейс, устойчивый к появлению новых видов движущихся объектов \- 1 балл  
4.3. Реализован тесты:  
\- Для объекта, находящегося в точке (12, 5\) и движущегося со скоростью (-7, 3\) движение меняет положение объекта на (5, 8\)  
\- Попытка сдвинуть объект, у которого невозможно прочитать положение в пространстве, приводит к ошибке  
\- Попытка сдвинуть объект, у которого невозможно прочитать значение мгновенной скорости, приводит к ошибке  
\- Попытка сдвинуть объект, у которого невозможно изменить положение в пространстве, приводит к ошибке  
1 балл  
5\. Поворот объекта вокруг оси.  
5.1. Сам поворот реализован в виде отдельного класса \- 1 балл  
5.2. Для поворачивающегося объекта определен интерфейс, устойчивый к появлению новых видов движущихся объектов \- 1 балл  
5.3. Реализован тесты \- 1 балл.

Прямолинейное движение:

* Интерфейс: `IMovable`  
* Методы:  
  * `Vector2 GetPosition()` \- возвращает текущие координаты объекта.  
  * `void SetPosition(Vector2 newPosition)` \- устанавливает новые координаты объекта.  
  * `Vector2 GetVelocity()` \- возвращает вектор мгновенной скорости (например, пикселей в секунду).

Любой будущий объект (корабль, торпеда, астероид), который должен двигаться, будет реализовывать этот интерфейс. Класс `MoveCommand` будет работать только с этим интерфейсом, а не с конкретными классами кораблей.

`Vector2 position = movable.GetPosition();   // Can throw`

`Vector2 velocity = movable.GetVelocity();   // Can throw`

`Vector2 newPosition = position + velocity * deltaTime;`

`movable.SetPosition(newPosition);           // Can throw`

Поворот:

* Интерфейс: `IRotatable`  
* Методы:  
  * `Vector2 GetDirection()` \- возвращает текущий вектор направления (нормализованный вектор).  
  * `void SetDirection(Vector2 newDirection)` \- устанавливает новый вектор направления.  
  * `int GetAngularVelocity()` \- возвращает угловую скорость (градусы или радианы в секунду). *Может пригодиться для непрерывного поворота, но по ТЗ требуется просто повернуть на угол.*  
  * `int GetRotationAngle()` \- возвращает угол, на который объект может повернуться за один ход. *Более точно соответствует формулировке "поворачиваться".*

Интерфейс `IRotatable` **не зависит** от `IMovable`, что позволяет иметь объекты, которые только поворачиваются (например, башня корабля), и объекты, которые делают и то, и другое (сам корабль).

`Vector2 currentDirection = rotatable.GetDirection();`

*`// Create a rotation matrix/quaternion or use formula for 2D rotation`*

`double angleRad = angleDegrees * Math.PI / 180.0;`

`double cos = Math.Cos(angleRad);`

`double sin = Math.Sin(angleRad);`

`Vector2 newDirection = new Vector2(`

            `x: currentDirection.X * cos - currentDirection.Y * sin,`
    
            `y: currentDirection.X * sin + currentDirection.Y * cos`

`);`

`rotatable.SetDirection(Vector2.Normalize(newDirection));`

## ДЗ 3 Механизм обработки исключений

Паттерны: **Команда**, **Цепочка обязанностей** (для обработчиков исключений) и **Стратегия** (разные команды-обертки реализуют разные стратегии обработки ошибок)

1\. ДЗ сдано на оценку \- 2 балла  
2\. Реализованы пункты 4-7. \- 2 балла.  
3\. Написаны тесты к пункту 4-7. \- 2 балла  
4\. Реализован пункт 8\. \- 1 балл  
5\. Написаны тесты к пункту 8\. \- 1 балл  
6\. Реализован пункт 9\. \- 1 балл  
7\. Написаны тесты к пункту 9\. \- 1 балл

1. Базовые сущности:  
   * `ICommand` \- интерфейс команды с методом `Execute()`.  
   * `Queue<ICommand>` \- очередь команд на выполнение.  
   * `ExceptionHandler` \- абстракция для обработчика исключений. Он будет принимать пару (команда, исключение) и возвращать новую команду для постановки в очередь (например, логгер или повторитель) или `null`, если исключение не обработано.  
2. Механизм обработки:  
   Основной цикл обработки очереди будет обернут в try-catch. Пойманное исключение будет передаваться по цепочке обработчиков, пока один из них не вернет команду для обработки ошибки. Эта команда добавляется в очередь.

`public interface ICommand {`

    `void Execute();`

`}`

`public interface IExceptionHandler {`

    `ICommand? Handle(ICommand command, Exception exception);`

`}`

### EventLoop:

private readonly Queue\<ICommand\> \_commandQueue \= new Queue\<ICommand\>();

private readonly List\<IExceptionHandler\> \_exceptionHandlers \= new List\<IExceptionHandler\>();

try  {

                command.Execute();

 }  catch (Exception ex) {

                ICommand? recoveryCommand \= null;
    
                foreach (var handler in \_exceptionHandlers) {
    
                    recoveryCommand \= handler.Handle(command, ex);
    
                    if (recoveryCommand \!= null) break;
    
                }
    
                if (recoveryCommand \!= null)  {
    
                    \_commandQueue.Enqueue(recoveryCommand);
    
                }

}

Стратегии:

// Обработчик для этой стратегии. Он будет ставить не оригинальную команду, а нашу обертку.

public class RetryOnceThenLogHandler : IExceptionHandler {

    private readonly ILogger \_logger;
    
    public RetryOnceThenLogHandler(ILogger logger) \=\> \_logger \= logger;
    
    public ICommand? Handle(ICommand command, Exception exception) {
    
        // Если команда УЖЕ является нашей оберткой, значит это вторая ошибка.
    
        // Мы должны вернуть команду для логирования.
    
        if (command is RetryOnceThenLogCommand retryCommand){
    
            return new LogExceptionCommand(retryCommand, exception, \_logger);
    
        }
    
        // Если это первая ошибка для этой команды, оборачиваем ее в нашу стратегию.
    
        return new RetryOnceThenLogCommand(command, \_logger);
    
    }

}

.// Обработчик для стратегии с двумя повторами

public class RetryTwiceThenLogHandler : IExceptionHandler {

    private readonly ILogger \_logger;
    
    public RetryTwiceThenLogHandler(ILogger logger) \=\> \_logger \= logger;
    
    public ICommand? Handle(ICommand command, Exception exception)  {
    
        if (command is RetryTwiceThenLogCommand retryCommand) {
    
            // Если команда уже была обернута и снова упала, проверяем счетчик.
    
            // Т.к. счетчик private, нам нужно либо сделать его public,
    
            // либо считать, что если команда уже обернута, значит это последняя попытка.
    
            return new LogExceptionCommand(retryCommand, exception, \_logger);
    
        }
    
        // Первая ошибка \- оборачиваем
    
        return new RetryTwiceThenLogCommand(command, \_logger);
    
    }

}

## ДЗ 4 Механизм обработки исключений

1\. Домашнее задание сдано \- 1 балл.  
2\. Реализована команда CheckFuelCommand \- 1балл  
3\. Написаны тесты к CheckFuelComamnd \- 1 балл  
4\. Реализована команда BurnFuelCommand \- 1балл  
5\. Написаны тесты к BurnFuelComamnd \- 1 балл  
6\. Реализована макрокоманда движения по прямой с расходом топлива и тесты к ней \- 1 балл  
7\. Написаны тесты к MacroComamnd \- 1 балл  
8\. Реализована команда ChangeVelocityCommand \- 1балл  
9\. Написаны тесты к ChangeVelocityComamnd \- 1 балл  
10\. Реализована команда поворота, которая еще и меняет вектор мгновенной скорости \- 1балл

Концепции:

1. Интерфейсы для объектов:  
   * `IFuelConsumer` \- объект, потребляющий топливо. Имеет свойства `FuelLevel` (текущий уровень) и `FuelConsumptionRate` (расход).  
   * `IMovable` \- объект, который может двигаться (как в предыдущем ДЗ: положение, скорость).  
   * `IRotatable` \- объект, который может поворачиваться (направление, угловая скорость).  
   * `IDirectionChangeable` \- объект, у которого можно изменить вектор скорости (новый интерфейс).  
2. Команды:  
   * `CheckFuelCommand` \- проверяет, достаточно ли топлива.  
   * `BurnFuelCommand` \- уменьшает уровень топлива.  
   * `ChangeVelocityCommand` \- изменяет вектор скорости объекта на основе его направления.  
   * `RotateWithChangeVelocityCommand` \- макрокоманда, объединяющая поворот и изменение скорости.  
   * `MoveWithFuelCommand` \- макрокоманда, объединяющая проверку топлива, движение и его сжигание.  
   * `MacroCommand` \- базовый класс для макрокоманд, который выполняет список команд и обрабатывает ошибки.

### Макрокоманда:

`public class MacroCommand : ICommand {`

    `protected readonly IList<ICommand> _commands;`
    
    `public MacroCommand(params ICommand[] commands){`
    
        `_commands = commands.ToList();`
    
    `}`
    
    `public virtual void Execute() {`
    
        `foreach (var command in _commands){`
    
            `command.Execute();`
    
        `}`
    
    `}`

`}`

### Команда для модификации вектора мгновенной скорости

public class ChangeVelocityCommand : ICommand

{

    private readonly IDirectionChangeable \_directionChangeable;
    
    private readonly double \_speedModifier; 

// Может быть 1, если скорость просто равна направлению

    public ChangeVelocityCommand(IDirectionChangeable directionChangeable, double speedModifier \= 1.0) {
    
        \_directionChangeable \= directionChangeable;
    
        \_speedModifier \= speedModifier;
    
    }
    
    public void Execute() {
    
        // Новая скорость \= нормализованное направление \* модуль старой скорости \* модификатор
    
        // Или более сложная логика. Предположим, что скорость должна совпадать с направлением.
    
        double magnitude \= \_directionChangeable.Velocity.Length() \* \_speedModifier;
    
        \_directionChangeable.Velocity \= Vector2.Normalize(\_directionChangeable.Direction) \* magnitude;
    
    }

}

### Команда поворота с изменением скорости

public class RotateWithChangeVelocityCommand : MacroCommand {

    // Эта макрокоманда выполняет поворот, а затем корректирует скорость
    
    public RotateWithChangeVelocityCommand(IRotatable rotatable, IDirectionChangeable directionChangeable)  : base(
    
            new RotateCommand(rotatable), 
    
            new ChangeVelocityCommand(directionChangeable)
    
        ) {  }

}

## ДЗ 5 IoC

В игре Космичекий бой есть набор операций над игровыми объектами: движение по прямой, поворот, выстрел. При этом содержание этих команд может отличаться для разных игр, в зависимости от того, какие правила игры были выбраны пользователями. Например, пользователи могут ограничить запас ход каждого корабля некоторым количеством топлива, а другой игре запретить поворачиваться кораблям по часовой стрелке и т.д.

IoC может помочь в этом случае, скрыв детали в стратегии разрешения зависимости.

Например,

IoC.Resolve("двигаться прямо", obj);

Возвращает команду, которая чаще всего является макрокомандой и осуществляет один шаг движения по прямой.

Реализовать IoC контейнер, который:

1. Разрешает зависимости с помощью метода, со следующей сигнатурой:

T IoC.Resolve(string key, params object\[\] args);

*Указание:* Если язык программирования не поддерживает Generics, как, например, PHP, то Запись Вам может быть незнакома. Так оеализуется параметрический полиморфизм в таких языках, как C++, C\#, Java, Kotlin и др.

Тогда просто возвращайте просто ссылку на базовый класс.

1. Регистрация зависимостей также происходит с помощью метода Resolve

2. IoC.Resolve("IoC.Register", "aaa", (args) \=\> new A()).Execute();  
3. Зависимости можно регистрировать в разных "скоупах"

IoC.Resolve("Scopes.New", "scopeId").Execute();

IoC.Resolve("Scopes.Current", "scopeId").Exceute();

*Указание:* Если Ваш фреймворк допускает работу с многопоточным кодом, то для работы со скоупами используйте ThreadLocal контейнер.

**Критерии оценивания:**

1. Интерфейс IoC устойчив к изменению требований. Оценка: 0 \- 3 балла (0 \- совсем не устойчив, 3 \- преподаватель не смог построить ни одного контрпримера)  
2. IoC предоставляет ровно один метод для всех операций. 1 балл  
3. IoC предоставляет работу со скоупами для предотвращения сильной связности. 2 балла.  
4. Реализованы модульные тесты. 2 балла  
5. Реализованы многопоточные тесты. 2 балла

Концепции:

1. Единый метод `Resolve`: Это ядро контейнера. Он работает как фасад и диспетчер.  
2. Система регистрации: Хранение стратегий создания объектов по строковому ключу.  
3. Скоупы (области видимости): Иерархическая система областей видимости зависимостей. Будем использовать `AsyncLocal` для thread-safe работы в асинхронном контексте.  
4. Встроенные команды: Команды для управления самим контейнером (`IoC.Register`, `Scopes.New`, etc.) регистрируются в самом контейнере.

// Хранилище стратегий: ключ \-\> фабрика

private static readonly Dictionary\<string, DependencyFactory\> \_dependencies \= new();

// Система скоупов. AsyncLocal для потокобезопасности.

private static readonly AsyncLocal\<Stack\<Dictionary\<string, DependencyFactory\>\>\> \_scopes \= new() {Value \= new Stack\<Dictionary\<string, DependencyFactory\>\>() };

    private static void RegisterBuiltInCommands() {
    
        \_dependencies\["IoC.Register"\] \= args \=\>  {
    
            string key \= (string)args\[0\];
    
            DependencyFactory factory \= (DependencyFactory)args\[1\];
    
            CurrentScope()\[key\] \= factory;
    
            return null;
    
        };
    
        \_dependencies\["Scopes.New"\] \= args \=\>  {
    
            string scopeId \= (string)args\[0\];
    
            var newScope \= new Dictionary\<string, DependencyFactory\>();
    
            \_scopes.Value.Push(newScope);
    
            return null;
    
        };
    
        \_dependencies\["Scopes.Current"\] \= args \=\> \_scopes.Value.Peek();
    
        // Другие встроенные команды...
    
    }

// Получение текущего активного скоупа

private static Dictionary\<string, DependencyFactory\> CurrentScope() {

        return \_scopes.Value.Count \> 0 ? \_scopes.Value.Peek() : \_dependencies;

}

### **Оценка устойчивости интерфейса**

Интерфейс чрезвычайно устойчив:

1. Единственный метод: `Resolve<T>(string key, params object[] args)`. Добавление новых функций не требует изменения сигнатур методов.  
2. Строковые ключи: Позволяют регистрировать любые зависимости без изменения API.  
3. Система скоупов: Позволяет изолировать конфигурации без изменения основного API.  
4. Параметры args: Фабрики могут принимать любое количество аргументов любой сложности.

## ДЗ 6 Генерация адаптеров по интерфейсу

Предположим, что у нас есть интерфейс

interface Spaceship.Operations.IMovable  
{  
  Vector getPosition();  
  void setPosition(Vector newValue);  
  Vector getVelocity();  
}

1\. Необходимо по нему сгенерировать следующий класс:

class AutoGenerated.MovableAdapter:  Spaceship.Operations.IMovable  
{  
  UObject obj;  
  public  MovableAdapter(UObject obj)  
  {    
     this.obj \= obj;  
  }

  public Vector getPosition()  
  {  
     return IoC.Resolve\<Vector\>("Spaceship.Operations.IMovable:position.get", obj);  
  }

  public Vector getVelocity()  
  {  
     return IoC.Resolve\<Vector\>("Spaceship.Operations.IMovable:velocity.get", obj);  
  }

  public void setPosition(Vector newValue)  
  {  
     IoC.Resolve\<ICommand\>("Spaceship.Operations.IMovable:position.set", obj, newValue).Execute();  
  }  
}

так, чтобы можно было создавать экземпляры этого класса.

Экземпляры генерируемых оберток для объекта obj можно создавать следующей строкой:

var adapter \= IoC.Resolve\<IMovable\>("Adapter", typeof(IMovable), obj);

3\*. Необязательная задача на подумать: если интерфейсе потребуются какие-либо методы, например, как

 interface Spaceship.Operations.IMovable  
{  
  Vector getPosition();  
  Vector setPosition(Vector newValue);  
  Vector getVelocity();

  void finish();  
}  
Придумать реализацию адаптера для подобных методов.

Критерии оценки:

1. Задание сдано на проверку \- 1 балл  
2. Реализован генератор адаптеров \- 3 балла  
3. Реализованы тесты на генератор адаптеров \- 2 балл  
4. Определена стратегия для IoC из п. 2 задания \- 2 балла  
5. Обработан случай из п. 3 задания \- 2 балла

### **Концепция**

1. Генератор адаптеров: Динамически создает классы, реализующие целевые интерфейсы, проксируя вызовы методов в IoC-контейнер.  
2. Стратегия IoC: Регистрация фабрики для создания адаптеров по ключу "Adapter".  
3. Обработка разных сигнатур методов: Анализ методов интерфейса для генерации правильного кода вызова IoC.

### Пример генератора

`@staticmethod`

    `def generate_adapter(interface_class, obj):`
    
        `class_name = f"{interface_class.__name__}Adapter"`
    
        `adapter_class = type(`
    
            `class_name,`
    
            `(interface_class,),`
    
            `{`
    
                `'_obj': obj,`
    
                `'__init__': AdapterGenerator._generate_constructor(),`
    
                `**AdapterGenerator._generate_methods(interface_class)`
    
            `}`
    
        `)`
    
        `return adapter_class(obj)`


​    

    `@staticmethod`
    
    `def _generate_constructor():`
    
        `def __init__(self, obj):`
    
            `self._obj = obj`
    
        `return __init__`
    
    `@staticmethod`
    
    `def _generate_methods(interface_class):`
    
        `methods = {}`
    
        `for method_name, method in interface_class.__dict__.items():`
    
            `if callable(method) and not method_name.startswith('__'):`
    
                `methods[method_name] = AdapterGenerator._generate_method(method_name, method)`
    
        `return methods`

Пример регистрации в IoC:

`IoC.register("Adapter", lambda interface_type, obj:` 

    `AdapterGenerator.generate_adapter(interface_type, obj))`

## ДЗ 7 Многопоточное выполнение команд

**Описание:**  
Предположим, что у нас есть набор команд, которые необходимо выполнить. Выполнение команд организуем в несколько потоков.  
Для этого будем считать, что у каждого потока есть своя потокобезопасная очередь.  
Для того, чтобы выполнить команду, ее необходимо добавить в очередь. Поток читает очередную команду из очереди и выполняет ее.  
Если выполнение команды прерывается выброшенным исключением, то поток должен отловить его и продолжить работу.  
Если сообщений нет в очереди, то поток засыпает до тех пор, пока очередь пуста.

Последовательность шагов решения:

Реализовать код, который запускается в отдельном потоке и делает следующее  
В цикле получает из потокобезопасной очереди команду и запускает ее.

1. Выброс исключения из команды не должен прерывать выполнение потока.

2. Написать команду, которая стартует код, написанный в пункте 1 в отдельном потоке.

3. Написать команду, которая останавливает цикл выполнения команд из пункта 1, не дожидаясь их полного завершения (hard stop).

4. Написать команду, которая останавливает цикл выполнения команд из пункта 1, только после того, как все команды завершат свою работу (soft stop).

5. Написать тесты на команду запуска и остановки потока.

**Критерии оценивания:**  
За выполнение каждого пункта, перечисленного ниже начисляются баллы:

1. ДЗ сдано на проверку \- 2 балла

2. Код решения опубликован на github/gitlab \- 1 балл

3. Настроен CI \- 2 балла

4. Код компилируется без ошибок \- 1 балл.

5. Написать тест, который проверяет, что после команды старт поток запущен \- 1балл и 4 балла \- если используются условные события синхронизации.

6. Написать тест, который проверяет, что после команды hard stop, поток завершается \- 1 балл

7. Написать тест, который проверяет, что после команды soft stop, поток завершается только после того, как все задачи закончились \- 2 балла

`while (!_queue.ShouldStop && !_queue.ShouldSoftStop) {`

  `try {`

    `var command = _queue.Dequeue(_queue.ShouldStop ?  new CancellationToken(true) : CancellationToken.None);`
    
    `…`

  `}`  
`}`


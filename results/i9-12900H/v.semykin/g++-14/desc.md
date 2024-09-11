## Производительность 

### Питание

Стоит также учесть, что если Вы выполняете бенчмарки на ноутбуке, используя аккумулятор, то производительность может просесть почти в 2 раза, если же ноутбук питается от сети, то производительность будет выше и соответственно скорость выполнения бенчмарков увеличится.

### Фоновые процессы

Дополнительно к этому, если устройство, на котором запущены бенчмарки, будет активно выполнять другие задачи, то производительность уменьшится, потому как планировщик будет выделять процессорное время не только на выполнение бенчмарков, но и на другие запущенные в системе процессы.

## Автоматизация

Я предлагаю такой **bash** скрипт, находящийся в моей директории `i9-12900H/v.semykin/g++-14/`, чтобы облегчить жизнь пользователям, которые хотят запустить Ваши бенчмарки у себя на устройстве, а также, чтобы автоматизировать этот процесс, не вводя по одной команде в терминал, где все они похожи.

## Virtual-inherit

Кажется Вы забыли добавить в root **CMakeLists.txt** поддиректорию `virtual-inherit/`, я ее добавил, затем столкнулся с ошибкой после компиляции:

```
[ 65%] Building CXX object virtual-inherit/CMakeFiles/virtinh.dir/virtinh.cc.o
/home/vladislavsemykin/Documents/benchmarks/virtual-inherit/virtinh.cc: In member function ‘virtual void CppBenchmark::__benchmark__43::Run(CppBenchmark::Context&)’:
/home/vladislavsemykin/Documents/benchmarks/virtual-inherit/virtinh.cc:45:11: error: cannot convert ‘Derived*’ to ‘VirtBase*’
   45 |   startup(&nv);
      |           ^~~
      |           |
      |           Derived*
/home/vladislavsemykin/Documents/benchmarks/virtual-inherit/virtinh.cc:34:49: note:   initializing argument 1 of ‘int startup(VirtBase*)’
   34 | int __attribute__((noinline)) startup(VirtBase *vb) {
      |                                       ~~~~~~~~~~^~
gmake[2]: *** [virtual-inherit/CMakeFiles/virtinh.dir/build.make:76: virtual-inherit/CMakeFiles/virtinh.dir/virtinh.cc.o] Error 1
gmake[1]: *** [CMakeFiles/Makefile2:483: virtual-inherit/CMakeFiles/virtinh.dir/all] Error 2
gmake: *** [Makefile:101: all] Error 2
```

Таким образом, необходимо было просто добавить такую же функцию `startup()` для класса `Base`:

```cpp
int __attribute__((noinline)) startup(Base *b) {
  int sum = 0;
  for (int i = 0; i < NBMK; ++i)
    sum += b->foo(NCALL);
  return sum;
}
```

Тогда все компилируется и выполняется верно.

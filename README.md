# Design Pattern Factory

Factory Design Pattern without first instantiation.

Most implementations of Factory require an instance for future use of the copy operator.
This version uses only the type of the object and the default operator.

### The goals of TFactory are:
- Mit Licence (see LICENSE)
- Zero dependencies (only STD)
- Only one file
- Portable
- Easy to use

### Simple sample of usage:
```C++
class Object
{
public:
	Object(void);
	virtual ~Object(void);
}

class MyObject1 : public Object
{
public:
	MyObject1(void);
}

class MyObject2 : public Object
{
public:
	MyObject2(void);
}

int main ()
{
	TFactory<Object> lMyFactory;

	lMyFactory.registerPlugin<MyObject1>("MyObject1");
	lMyFactory.registerPlugin<MyObject2>("MyObject2");

	std::shared_ptr<Object> lObj1SPtr = lMyFactory.makeSharedNewInstance("MyObject1");
	std::unique_ptr<Object> lObj2UPtr = lMyFactory.makeUniqueNewInstance("MyObject2");
}
```

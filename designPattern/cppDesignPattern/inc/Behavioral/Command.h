#pragma once

#include <iostream>
#include <vector>
using namespace std;

namespace nsNotUseCommand {
    class Giant
    {
    public:
        enum Type
        {
            Fee, Phi, Pheaux
        };
        Giant()
        {   
            m_id = s_next++;
            //cout << "\n m_id = " << m_id << endl;
            m_type = (Type)(m_id % 3);
        }
        Type get_type()
        {
            return m_type;
        }
        void fee()
        {
            cout << m_id << "-fee  ";
        }
        void phi()
        {
            cout << m_id << "-phi  ";
        }
        void pheaux()
        {
            cout << m_id << "-pheaux  ";
        }
    private:
        Type m_type;
        int m_id;
        static int s_next;
    };
    int Giant::s_next = 0;

    template <typename T> class Queue
    {
    public:
        Queue()
        {
            m_add = m_remove = 0;
        }
        void enque(T* c)
        {
            m_array[m_add] = c;
            m_add = (m_add + 1) % SIZE;
        }
        T* deque()
        {
            int temp = m_remove;
            m_remove = (m_remove + 1) % SIZE;
            return m_array[temp];
        }
    private:
        enum
        {
            SIZE = 8
        };
        T* m_array[SIZE];
        int m_add, m_remove;
    };
    
    void programTest()
    {
        Queue<Giant> que;
        Giant input[6], * bad_guy;

        for (int i = 0; i < 6; i++)
            que.enque(&input[i]);

        for (int i = 0; i < 6; i++)
        {
            bad_guy = que.deque();
            if (bad_guy->get_type() == Giant::Fee)
                bad_guy->fee();
            else if (bad_guy->get_type() == Giant::Phi)
                bad_guy->phi();
            else if (bad_guy->get_type() == Giant::Pheaux)
                bad_guy->pheaux();
        }
        cout << '\n';

    }
}

namespace nsExpantionTest {
    void bar() { cout << "void bar is called\n"; };

    class Foo
    {
    public:
        void bar() { cout << "Foo bar be called\n"; };
    };
    /*explains the principle of using "typedef void (*ptr)"*/

    //CASE 1

    typedef void (Foo::* ptr1)();
    typedef void (*ptr2)();

    //CASE 2 not use set-name
    void (Foo::*ptr31)() = &Foo::bar;
    void (Foo::*ptr32)(); //Prototype
    
    void (*ptr41)() = &bar;
    void (*ptr42)();
    void programTest() {
        cout << "\nCASE1";
        cout << "\n";
        Foo* m_objectfoo = new Foo;
        ptr1 foo_bar_fn = &(Foo::bar);
        (m_objectfoo->*foo_bar_fn) ();

        cout << "\n";
        ptr2 bar_fn = &bar;
        bar_fn(); // call bar()
        (*bar_fn) (); // like a same above

        //CASE 2
        cout << "\nCASE2";
        cout << "\n";
        (m_objectfoo->*ptr31)();
        ptr32 = &(Foo::bar);// JUST USE IN OTHER FUNCTION or .cpp file "ex: programTest()"
        (m_objectfoo->*ptr32)();

        cout << "\n";
        ptr41();
        ptr42 = &(bar);// JUST USE IN FUNCTION or .cpp file "ex: programTest()"
        ptr42();

    }
}

namespace nsCommand {
    class Giant
    {
    public:
        Giant()
        {
            m_id = s_next++;
        }
        ~Giant() { /*Add 241019*/
            cout << "\n Giant is called \n";
        }
        void fee()
        {
            cout << m_id << "-fee  ";
        }
        void phi()
        {
            cout << m_id << "-phi  ";
        }
        void pheaux()
        {
            cout << m_id << "-pheaux  ";
        }
    private:
        int m_id;
        static int s_next;
    };
    int Giant::s_next = 0;

    class Command
    {
    public:
        typedef void(Giant::* Action)();
        Command(Giant* object, Action method)
        {
            m_object = object;
            m_method = method;
        }
        ~Command() { /*Add 241019*/
            cout << "\n Distuctor is called \n";
            delete m_object;
        }
        void execute()
        {
            (m_object->*m_method)();
        }
    private:
        Giant* m_object;
        Action m_method;
    };

    template <typename T> class Queue
    {
    public:
        Queue()
        {
            m_add = m_remove = 0;
        }
        void enque(T* c)
        {
            m_array[m_add] = c;
            c->execute();
            m_add = (m_add + 1) % SIZE;
        }
        T* deque()
        {
            int temp = m_remove;
            m_remove = (m_remove + 1) % SIZE;
            return m_array[temp];
        }
    private:
        enum
        {
            SIZE = 8 //12 /*Add 241019*/
        };
        T* m_array[SIZE];
        int m_add, m_remove;
    };

    void programTest()
    {
        Queue<Command> que;
        Command* input[] =
        {
          new Command(new Giant, &Giant::fee), 
            new Command(new Giant, &Giant::phi),
                new Command(new Giant, &Giant::pheaux), 
                    new Command(new Giant, &Giant::fee), 
                        new Command(new Giant, &Giant::phi), 
                            new Command(new Giant, &Giant::pheaux),
        };

        for (int i = 0; i < 6; i++)
            que.enque(input[i]);
        for (int i = 5; i >= 0; i--)
            que.enque(input[i]);
        cout << endl;
        for (int i = 0; i < 12; i++)/*Add 241019*/
            que.deque()->execute();
        cout << '\n';
        for(auto &i : input) delete i;/*Add 241019*/
        
    }
}

namespace nsMacroCommand {
    class Number
    {
    public:
        void doubleVal(int& value)
        {
            value *= 2;
        }
    };

    class Command
    {
    public:
        virtual void execute(int&) = 0;
    };

    class SimpleCommand : public Command
    {
        typedef void(Number::* Action)(int&);
        Number* receiver;
        Action action;
    public:
        SimpleCommand(Number* rec, Action act)
        {
            receiver = rec;
            action = act;
        }
        /*virtual*/void execute(int& num)
        {
            (receiver->*action)(num);
        }
    };

    class MacroCommand : public Command
    {
        vector < Command* > list;
    public:
        void add(Command* cmd)
        {
            list.push_back(cmd);
        }
        /*virtual*/void execute(int& num)
        {
            for (int i = 0; i < list.size(); i++) //0-2
                list[i]->execute(num);
        }
    };

    void programTest()
    {
        Number object;
        
        Command* commands[4];
        //commands[0] = &SimpleCommand(&object, &Number::doubleVal); //error
        commands[0] = new SimpleCommand(&object, &Number::doubleVal);// call excute x2 //fix

        MacroCommand two;
        two.add(commands[0]);
        two.add(commands[0]); //list size = 2 ->x4
        commands[1] = &two;
        //List two  &Simp &Simp

        MacroCommand four;
        four.add(&two);
        four.add(&two);
        commands[2] = &four;        
        //List four &Simp &Simp &Simp &Simp

        MacroCommand fiveX;
        fiveX.add(&two);
        fiveX.add(commands[0]);
        commands[3] = &fiveX;

        int num, index;
        while (true)
        {
            cout << "Enter number selection (0=>2x 1=>4x 2=>16x 3=>5x): ";
            cin >> num >> index;
            if (index < 0 || index > 4) break;
            commands[index]->execute(num);
            cout << "   " << num << '\n';
            
        }

        delete commands[0];
    }
}

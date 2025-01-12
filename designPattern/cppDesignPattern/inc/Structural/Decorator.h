#pragma once

#include <iostream>

namespace nsDecorator {
    class IDraw {
    public:
        virtual ~IDraw() {}
        virtual void do_it() = 0;
    };
    // Draw TYPE
    class DrawStraigth : public IDraw {
    public:
        ~DrawStraigth() {
            std::cout << "DrawStraigth dtor" << '\n';
        }
        /*virtual*/
        void do_it() {
            std::cout << "DrawStraigth in axists: ";
        }
    };

    class DrawPolyline : public IDraw {
    public:
        ~DrawPolyline() {
            std::cout << "DrawPolyline dtor" << '\n';
        }
        /*virtual*/
        void do_it() {
            std::cout << "DrawPolyline in axists: ";
        }
    };
    //-------------------------------------------------------
    // Decorator layer
    class DrawAbstract : public IDraw {
    public:
        DrawAbstract(IDraw* inner) {
            m_wrappee = inner;
        }
        ~DrawAbstract() {
            delete m_wrappee;
        }
        /*virtual*/
        void do_it() {
            m_wrappee->do_it();
        }
    private:
        IDraw* m_wrappee;
    };

    class X : public DrawAbstract {
    public:
        X(IDraw* core) : DrawAbstract(core) {}
        ~X() {
            std::cout << "X dtor" << "   ";
        }
        /*virtual*/
        void do_it() {
            DrawAbstract::do_it();
            std::cout << 'X';
        }
    };

    class Y : public DrawAbstract {
    public:
        Y(IDraw* core) : DrawAbstract(core) {}
        ~Y() {
            std::cout << "Y dtor" << "   ";
        }
        /*virtual*/
        void do_it() {
            DrawAbstract::do_it();
            std::cout << 'Y';
        }
    };

    class Z : public DrawAbstract {
    public:
        Z(IDraw* core) : DrawAbstract(core) {}
        ~Z() {
            std::cout << "Z dtor" << "   ";
        }
        /*virtual*/
        void do_it() {
            DrawAbstract::do_it();
            std::cout << 'Z';
        }
    };

    void programTest() {
        IDraw* anX = new X(new DrawStraigth);
        IDraw* anXY = new Y(new X(new DrawPolyline));
        IDraw* anXYZ = new Z(new Y(new X(new DrawStraigth)));
        IDraw* anXXY = new Y(new X(new X(new DrawPolyline)));
        anX->do_it();
        std::cout << '\n';
        anXY->do_it();
        std::cout << '\n';
        anXYZ->do_it();
        std::cout << '\n';
        anXXY->do_it();
        std::cout << '\n';
        delete anX;
        delete anXY;
        delete anXYZ;
        delete anXXY;
    }
}

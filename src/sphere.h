/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     
 * Purpose:     
 * Author:      
 * License:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __SRC_SPHERE_H_INCLUDED__
#define __SRC_SPHERE_H_INCLUDED__

#include <pacalib/pacalib.h>
#include <glesly/surfaced-icosahedron.h>
#include <Debug/Debug.h>

SYS_DECLARE_MODULE(DM_GLESLY);

namespace Glesly
{
    class SphereSurface;

    namespace SphereData
    {
        class Draw: public PaCaLib::Draw
        {
            friend class SphereSurface;

         public:
            Draw(SphereSurface & parent);

         protected:
            SphereSurface & parent;

            virtual void Scale(float w, float h) override;
            virtual void SetColour(float r, float g, float b, float a) override;
            virtual void SetColourCompose(PaCaLib::ColourCompose mode = PaCaLib::COLOUR_COMPOSE_DEFAULT) override;
            virtual void SetTextOutlineColour(float r, float g, float b, float a = 1.0) override;
            virtual void SetTextOutline(float outline) override;
            virtual void SetLineWidth(float width) override;
            virtual void SetLineCap(PaCaLib::LineCap mode) override;
            virtual void Paint(void) override;
            virtual PaCaLib::PathPtr NewPath(void) override;
            virtual float DrawTextInternal(float x, float y, PaCaLib::TextMode mode, const char * text, float size, float offset, float aspect, float rotation) override;

            PaCaLib::DrawPtr draws[6];

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::SphereSurface::Draw");

        }; // class Glesly::SphereData::Draw

        class Path: public PaCaLib::Path
        {
            friend class SphereData::Draw;

            static constexpr int MAX_OPERS = 100;

            enum Opcode
            {
                NO_OP       = 0,
                OP_MOVE,
                OP_LINE,
                OP_ARC,
                OP_BEZIER,
                OP_CLOSE
            };

            struct Oper
            {
                uint32_t    op;

                float       data[5];

            }; // struct Glesly::SphereData::Path::Oper

            inline void push(Opcode op, float d1 = 0.0f, float d2 = 0.0f, float d3 = 0.0f, float d4 = 0.0f, float d5 = 0.0f)
            {
                ASSERT(opCount < MAX_OPERS, "too many opearions on a Glesly::SphereData::Path");

                Oper & o = opcodes[opCount++];
                o.op = op;
                o.data[0] = d1;
                o.data[1] = d2;
                o.data[2] = d3;
                o.data[3] = d4;
                o.data[4] = d5;
            }

         public:
            Path(SphereData::Draw & parent);

         protected:
            SphereData::Draw & parent;

            virtual void Stroke(void) override;
            virtual void Fill(void) override;

            virtual void Move(float x, float y) override
            {
                push(OP_MOVE, x, y);
            }

            virtual void Line(float x, float y) override
            {
                push(OP_LINE, x, y);
            }

            virtual void Arc(float xc, float yc, float r, float a1, float a2) override
            {
                push(OP_ARC, xc, yc, r, a1, a2);
            }

            virtual void Bezier(float x, float y, float dx, float dy) override
            {
                push(OP_BEZIER, x, y, dx, dy);
            }

            virtual void Close(void) override
            {
                push(OP_CLOSE);
            }

            virtual void Clear(void) override
            {
                opCount = 0;
            }

            Oper    opcodes[MAX_OPERS];

            int     opCount;

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::SphereSurface::Path");

        }; // class Glesly::SphereData::Path

    } // namespace Glesly::SphereData

    /// Cubemap surfaces for an OpenGL Sphere
    /*! This class represents a cube-mapped sphere object.<br>
     *  Note that the textures are not created by default, one of the \ref SphereSurface::reset(int, Glesly::PixelFormat)
     *  or \ref SphereSurface::reset(const char * const *) functions must be called to do it. */
    class SphereSurface
    {
     protected:
        inline SphereSurface(int size, Glesly::PixelFormat format = Glesly::FORMAT_DEFAULT):
            textureTargets { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
            myFormat(format)
        {
            if (size) {
                reset(size, format);
            }
        }

        PaCaLib::TargetPtr pacaTargets[6];

        Glesly::Target2D * textureTargets[6];

        Glesly::PixelFormat myFormat;

     public:
        void reset(int size, Glesly::PixelFormat format = Glesly::FORMAT_DEFAULT);
        void reset(const char * const * filenames);

        /// Return a \ref PaCaLib::Draw instance
        /*! The surface of the sphere can be drawn using this interface. The x and y parameters of the
         *  drawing functions are angles (longitude and latitude) in radian. */
        PaCaLib::DrawPtr Draw(void);

        inline PaCaLib::DrawPtr GetDraw(int index)
        {
            ASSERT(index >= 0 && index < 6, "target index overflow: " << index);
            ASSERT(pacaTargets[index], "no target for index " << index);
            return pacaTargets[index]->Draw();
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::SphereSurface");

        void reset(PaCaLib::TargetPtr & target, const char * name, int & size);
        void updatePointers(void);

    }; // class Glesly::SphereSurface

    /// OpenGL Sphere object, with drawing capabilities
    template <int resolution = 0>
    class Sphere: public SphereSurface, public Glesly::SurfacedIcosahedron<resolution>
    {
        typedef Glesly::SurfacedIcosahedron<resolution> super;

     protected:
        Sphere(Glesly::Render & render, float radius, int size):
            SphereSurface(size),
            super(render, radius)
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

     public:
        inline static Glesly::ObjectPtr Create(Glesly::Render & render, float radius, int size = 0)
        {
            return (new Sphere(render, radius, size))->ObjectBase::Create();
        }

        virtual ~Sphere()
        {
            SYS_DEBUG_MEMBER(DM_GLESLY);
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Sphere");

        virtual const Glesly::Target2D * const * getTargets(void) const override
        {
            return textureTargets;
        }

    }; // class Glesly::Sphere

} // namespace Glesly

#endif /* __SRC_SPHERE_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */

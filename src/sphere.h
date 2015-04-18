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
        class Convert3D
        {
         protected:
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
                uint16_t    op;

                uint16_t    flag;

                float       data[6];

            }; // struct Glesly::SphereData::Path::Oper

            void Convert(float longitude, float latitude, float position[3]);
            void Convert(const Oper * source, Oper * destination, int count);

        }; // class Glesly::SphereData::Convert3D

        class Draw: public PaCaLib::Draw, public Convert3D
        {
            friend class SphereSurface;

         public:
            Draw(SphereSurface & parent);

            void Stroke(const Oper * ops, int count);
            void Fill(const Oper * ops, int count);

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
            virtual float DrawTextInternal(float lon, float lat, PaCaLib::TextMode mode, const char * text, float size, float offset, float aspect, float rotation, float shear_x, float shear_y) override;

            PaCaLib::DrawPtr draws[6];

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::SphereSurface::Draw");

            void Stroke(int index, const Oper * ops, int count);
            void Fill(int index, const Oper * ops, int count);
            float DrawTextInternal(float x, float y, float z, int index, PaCaLib::TextMode mode, const char * text, float size, float offset, float aspect, float rotation, float shear_x, float shear_y);

        }; // class Glesly::SphereData::Draw

        class Path: public PaCaLib::Path, public Convert3D
        {
            friend class SphereData::Draw;

            inline void push(Opcode op)
            {
                ASSERT(opCount < MAX_OPERS, "too many opearions on a Glesly::SphereData::Path");

                Oper & o = opcodes[opCount++];
                o.op = op;
                o.data[0] = 0.0f;
                o.data[1] = 0.0f;
                o.data[2] = 0.0f;
                o.data[3] = 0.0f;
                o.data[4] = 0.0f;
                o.data[5] = 0.0f;
            }

            inline void push(Opcode op, float * c, float d1 = 0.0f, float d2 = 0.0f, float d3 = 0.0f)
            {
                ASSERT(opCount < MAX_OPERS, "too many opearions on a Glesly::SphereData::Path");

                Oper & o = opcodes[opCount++];
                o.op = op;
                o.data[0] = c[0];
                o.data[1] = c[1];
                o.data[2] = c[2];
                o.data[3] = d1;
                o.data[4] = d2;
                o.data[5] = d3;
            }

         public:
            Path(SphereData::Draw & parent);

         protected:
            SphereData::Draw & parent;

            virtual void Move(float x, float y) override;
            virtual void Line(float x, float y) override;
            virtual void Arc(float xc, float yc, float r, float a1, float a2) override;
            virtual void Bezier(float x, float y, float dx, float dy) override;
            virtual void Close(void) override;
            virtual void Clear(void) override;
            virtual void Stroke(void) override;
            virtual void Fill(void) override;

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
            SYS_DEBUG_MEMBER(DM_GLESLY);
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

        PaCaLib::DrawPtr Draw(void);

        /// Return a \ref PaCaLib::DrawPtr for the specified surface of the sphere
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

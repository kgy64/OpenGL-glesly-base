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
    class SphereBitmaps;

    namespace SphereData
    {
        class Draw: public PaCaLib::Draw
        {
            friend class SphereBitmaps;

         public:
            Draw(SphereBitmaps & parent);

         protected:
            SphereBitmaps & parent;

            virtual void Scale(float w, float h) override;
            virtual void SetColour(float r, float g, float b, float a) override;
            virtual void SetColourCompose(PaCaLib::ColourCompose mode = PaCaLib::COLOUR_COMPOSE_DEFAULT) override;
            virtual void SetTextOutlineColour(float r, float g, float b, float a = 1.0) override;
            virtual void SetTextOutline(float outline) override;
            virtual void SetLineWidth(float width) override;
            virtual void SetLineCap(PaCaLib::LineCap mode) override;
            virtual void Paint(void) override;
            virtual PaCaLib::PathPtr NewPath(void) override;
            virtual float DrawTextInternal(float x, float y, PaCaLib::TextMode mode, const char * text, float size, float offset, float aspect = 1.0) override;

            PaCaLib::DrawPtr draws[6];

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::SphereBitmaps::Draw");

        }; // class Glesly::SphereData::Draw

        class Path: public PaCaLib::Path
        {
            friend class SphereData::Draw;

            static constexpr int MAX_OPERS = 100;

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

            Oper    opcodes[MAX_OPERS];

            int     opCount;

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::SphereBitmaps::Path");

            void push(Opcode op, float d1 = 0.0f, float d2 = 0.0f, float d3 = 0.0f, float d4 = 0.0f, float d5 = 0.0f);

        }; // class Glesly::SphereData::Path

    } // namespace Glesly::SphereData

    class SphereBitmaps
    {
     protected:
        inline SphereBitmaps(int size, Glesly::PixelFormat format = Glesly::FORMAT_DEFAULT):
            textureTargets { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
            pacaTargets { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
            myFormat(format)
        {
            if (size) {
                reset(size, format);
            }
        }

        PaCaLib::TargetPtr texture_0;
        PaCaLib::TargetPtr texture_1;
        PaCaLib::TargetPtr texture_2;
        PaCaLib::TargetPtr texture_3;
        PaCaLib::TargetPtr texture_4;
        PaCaLib::TargetPtr texture_5;

        Glesly::Target2D * textureTargets[6];
        PaCaLib::Target * pacaTargets[6];

        Glesly::PixelFormat myFormat;

     public:
        void reset(int size, Glesly::PixelFormat format = Glesly::FORMAT_DEFAULT);
        void reset(const char * const * filenames);
        PaCaLib::DrawPtr Draw(void);

        inline PaCaLib::DrawPtr GetDraw(int index)
        {
            ASSERT(index >= 0 && index < 6, "target index overflow: " << index);
            ASSERT(pacaTargets[index], "no target for index " << index);
            return pacaTargets[index]->Draw();
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::SphereBitmaps");

        void reset(PaCaLib::TargetPtr & target, const char * name, int & size);
        void updatePointers(void);

    }; // class Glesly::SphereBitmaps

    template <int resolution = 0>
    class Sphere: public SphereBitmaps, public Glesly::SurfacedIcosahedron<resolution>
    {
        typedef Glesly::SurfacedIcosahedron<resolution> super;

     protected:
        Sphere(Glesly::Render & render, float radius, int size):
            SphereBitmaps(size),
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

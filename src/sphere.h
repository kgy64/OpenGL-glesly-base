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

#include <ostream>

SYS_DECLARE_MODULE(DM_GL_SPHERE);

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

            enum _Status
            {
                ST_UNUSED       =   0,
                ST_USED         =   1,
                ST_ADDED        =   2,
            };

         public:
            struct Oper
            {
                inline Oper(void)
                {
                    Invalidate();
                }

                VIRTUAL_IF_DEBUG inline ~Oper()
                {
                }

                inline bool isUsed(void) const
                {
                    return flag != ST_UNUSED;
                }

                inline void Invalidate(void)
                {
                    op = NO_OP;
                    flag = 0;
                    memset(data, 0, sizeof(data));  // ugly :-(
                }

                void Prepare(void);
                void Draw(PaCaLib::PathPtr & path, bool continuous) const;

                uint16_t    op;

                uint16_t    flag;

                float       data[6];

                void toStream(std::ostream & os) const;

             private:
                SYS_DEFINE_CLASS_NAME("Glesly::SphereData::Convert3D::Oper");

            }; // struct Glesly::SphereData::Convert3D::Oper

            /// Converts longitude and latitude to 3D positions
            struct Position
            {
                Position(float lon, float lat);

                /// The raw cos(latitude) value
                float cos_lat;

                /// The raw cos(longitude) value
                float cos_lon;

                /// X axis: points to East
                float x;

                /// Y axis: points to North
                float y;

                /// Z axis: points to Front (Africa)
                float z;

            }; // struct Glesly::SphereData::Convert3D::Position

         protected:
            void Convert(const Oper * source, Oper * destination, int count, int mode);

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::SphereData::Convert3D");

        }; // class Glesly::SphereData::Convert3D

        class Draw: public PaCaLib::Draw, public Convert3D
        {
            friend class SphereSurface;

         public:
            Draw(SphereSurface & parent);

            void DrawPath(PaCaLib::Path::DrawMode mode, const Oper * ops, int count);

         protected:
            SphereSurface & parent;

            virtual void Scale(float w, float h) override;
            virtual void SetColourCompose(PaCaLib::ColourCompose mode = PaCaLib::COLOUR_COMPOSE_DEFAULT) override;
            virtual void SetColour(float r, float g, float b, float a) override;
            virtual void SetOutlineColour(float r, float g, float b, float a) override;
            virtual void SetOutlineWidth(float outline) override;
            virtual void SetLineWidth(float width) override;
            virtual void SetLineCap(PaCaLib::LineCap mode) override;
            virtual void Paint(void) override;
            virtual PaCaLib::PathPtr NewPath(void) override;
            virtual float DrawTextInternal(const PaCaLib::Draw::TextParams & params, const PaCaLib::Draw::Distortion * distortion) override;

            PaCaLib::DrawPtr draws[6];

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::SphereData::Draw");

            void DrawPath(PaCaLib::Path::DrawMode mode, int index, const Oper * op, int count);
            float DrawTextInternal(const PaCaLib::Draw::TextParams & params, PaCaLib::Draw::Distortion & distortion, float x, float y, float z, float corr, int index);

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

            inline void push(Opcode op, const Position & pos, float d1 = 0.0f, float d2 = 0.0f, float d3 = 0.0f)
            {
                ASSERT(opCount < MAX_OPERS, "too many opearions on a Glesly::SphereData::Path");

                Oper & o = opcodes[opCount++];
                o.op = op;
                o.data[0] = pos.x;
                o.data[1] = pos.y;
                o.data[2] = pos.z;
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
            virtual void Arc(float x, float y, float r, float a1, float a2) override;
            virtual void Bezier(float x, float y, float dx, float dy) override;
            virtual void Close(void) override;
            virtual void Clear(void) override;
            virtual void Draw(DrawMode mode) override;

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
            SYS_DEBUG_MEMBER(DM_GL_SPHERE);
        }

     public:
        inline static Glesly::ObjectPtr Create(Glesly::Render & render, float radius, int size = 0)
        {
            return (new Sphere(render, radius, size))->ObjectBase::Create();
        }

        virtual ~Sphere()
        {
            SYS_DEBUG_MEMBER(DM_GL_SPHERE);
        }

     private:
        SYS_DEFINE_CLASS_NAME("Glesly::Sphere");

        virtual const Glesly::Target2D * const * getTargets(void) const override
        {
            return textureTargets;
        }

    }; // class Glesly::Sphere

} // namespace Glesly

static inline std::ostream & operator<<(std::ostream & os, const Glesly::SphereData::Convert3D::Oper & op)
{
 op.toStream(os);
 return os;
}

#endif /* __SRC_SPHERE_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */

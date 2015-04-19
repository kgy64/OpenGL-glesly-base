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
#include <Memory/Memory.h>
#include <Debug/Debug.h>

SYS_DECLARE_MODULE(DM_GL_SPHERE);

namespace Glesly
{
    class SphereSurface;

    namespace SphereData
    {
        class Convert3D
        {
         public:

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

             private:
                SYS_DEFINE_CLASS_NAME("Glesly::SphereData::Convert3D::Position");

            }; // struct Glesly::SphereData::Convert3D::Position

            class Operations;

            typedef MEM::shared_ptr<Operations> OperPtr;

            class Operations
            {
             public:

                struct OneOp;

                typedef OneOp * iterator;
                typedef const OneOp * const_iterator;

                inline Operations(void):
                    opCount(0)
                {
                }

                Operations(const_iterator start, const_iterator _end);
                Operations(const Operations & original, int mode);

                VIRTUAL_IF_DEBUG ~Operations();

                static OperPtr Create(void)
                {
                    return OperPtr(new Operations());
                }

                static OperPtr Create(const_iterator start, const_iterator _end)
                {
                    return OperPtr(new Operations(start, _end));
                }

                struct OneOp
                {
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
                        /// The position is stored on another bitmap
                        ST_UNUSED       =   0,

                        /// The position is not on this bitmap, but near enough
                        /*! It is necessary to interpolate up to the edges. */
                        ST_OUT_OF_RANGE =   1,

                        /// The position is on this bitmap
                        ST_USED         =   2,
                    };

                    inline OneOp(void)
                    {
                        Invalidate();
                    }

                    VIRTUAL_IF_DEBUG inline ~OneOp()
                    {
                    }

                    inline bool isValid(void) const
                    {
                        return flag != ST_UNUSED;
                    }

                    inline bool isUsed(void) const
                    {
                        return flag == ST_USED;
                    }

                    inline void Invalidate(void)
                    {
                        op = NO_OP;
                        flag = ST_UNUSED;
                        x = 0.0f;
                        y = 0.0f;
                        z = 0.0f;
                        u.clear();
                    }

                    void Prepare(int mode, const OneOp & src);
                    void Draw(PaCaLib::PathPtr & path, bool continuous = true) const;

                    uint16_t    op;

                    uint16_t    flag;

                    float   x;
                    float   y;
                    float   z;

                    union _u {
                        struct {
                            float   radius;
                            float   start;
                            float   end;
                        } arc;

                        struct {
                            float   dx;
                            float   dy;
                        } bezier;

                        inline void clear(void)
                        {
                            arc.radius = 0.0f;
                            arc.start = 0.0f;
                            arc.end = 0.0f;
                        }

                        void toStream(std::ostream & os) const;

                    } u; // union Glesly::SphereData::Convert3D::OneOp::_u

                    void toStream(std::ostream & os) const;

                 private:
                    SYS_DEFINE_CLASS_NAME("Glesly::SphereData::Convert3D::Operations::OneOp");

                }; // struct Glesly::SphereData::Convert3D::Operations::OneOp

                inline int size(void) const
                {
                    return opCount;
                }

                inline OneOp * begin(void)
                {
                    return opcodes;
                }

                inline const OneOp * cbegin(void) const
                {
                    return opcodes;
                }

                inline OneOp * end(void)
                {
                    ASSERT(opCount < MAX_OPERS, "Too many operations: " << opCount);
                    return opcodes + opCount;
                }

                inline const OneOp * cend(void) const
                {
                    ASSERT(opCount < MAX_OPERS, "Too many operations: " << opCount);
                    return opcodes + opCount;
                }

                inline void Clear(void)
                {
                    opCount = 0;
                }

                inline void Move(const Position & pos)
                {
                    push(OneOp::OP_MOVE, pos);
                }

                inline void Line(const Position & pos)
                {
                    push(OneOp::OP_LINE, pos);
                }

                inline void Arc(const Position & pos, float r, float a1, float a2)
                {
                    push(OneOp::OP_ARC, pos, r, a1, a2);
                }

                inline void Bezier(const Position & pos, float dx, float dy)
                {
                    push(OneOp::OP_BEZIER, pos, dx, dy);
                }

                inline void Close(void)
                {
                    push(OneOp::OP_CLOSE);
                }

                inline void push(const_iterator data)
                {
                    opcodes[opCount++] = *data;
                }

                void append(const Operations & other);
                void append(const_iterator start, const_iterator _end);
                const OperPtr * getSubOperations(void) const;
                void toStream(std::ostream & os) const;

             protected:
                void push(OneOp::Opcode op)
                {
                    ASSERT(opCount < MAX_OPERS, "too many opearions on a Glesly::SphereData::Path");

                    OneOp & o = opcodes[opCount++];
                    o.op = op;
                    o.x = 0.0f;
                    o.y = 0.0f;
                    o.z = 0.0f;
                    o.u.clear();
                }

                void push(OneOp::Opcode op, const Position & pos, float d1 = 0.0f, float d2 = 0.0f, float d3 = 0.0f)
                {
                    ASSERT(opCount < MAX_OPERS, "too many opearions on a Glesly::SphereData::Path");

                    OneOp & o = opcodes[opCount++];
                    o.op = op;
                    o.x = pos.x;
                    o.y = pos.y;
                    o.z = pos.z;
                    o.u.arc = { d1, d2, d3 };
                }

                static constexpr int MAX_OPERS = 100;
                static constexpr int MAX_LOOPS = 20;

                OneOp   opcodes[MAX_OPERS];

                int     opCount;

                mutable OperPtr sub_opers[MAX_LOOPS+1];

             private:
                SYS_DEFINE_CLASS_NAME("Glesly::SphereData::Convert3D::Operations");

            }; // class Glesly::SphereData::Convert3D::Operations

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::SphereData::Convert3D");

        }; // class Glesly::SphereData::Convert3D

        class Draw: public PaCaLib::Draw, public Convert3D
        {
            friend class SphereSurface;

         public:
            Draw(SphereSurface & parent);

            void DrawPath(PaCaLib::Path::DrawMode mode, const Operations & ops);

         protected:
            SphereSurface & parent;

            virtual void Scale(float w, float h) override;
            virtual void SetColour(float r, float g, float b, float a) override;
            virtual void SetColourCompose(PaCaLib::ColourCompose mode = PaCaLib::COLOUR_COMPOSE_DEFAULT) override;
            virtual void SetOutlineColour(float r, float g, float b, float a = 1.0) override;
            virtual void SetOutlineWidth(float outline) override;
            virtual void SetLineWidth(float width) override;
            virtual void SetLineCap(PaCaLib::LineCap mode) override;
            virtual void Paint(void) override;
            virtual PaCaLib::PathPtr NewPath(void) override;
            virtual float DrawTextInternal(const TextParams & params, const Distortion * distortion = nullptr) override;

            PaCaLib::DrawPtr draws[6];

         private:
            SYS_DEFINE_CLASS_NAME("Glesly::SphereSurface::Draw");

            void DrawPath(PaCaLib::Path::DrawMode mode, int index, const Operations & ops);
            bool DrawFillOnly(PaCaLib::Path::DrawMode mode, int index, const Operations & ops);
            float DrawTextInternal(const PaCaLib::Draw::TextParams & params, PaCaLib::Draw::Distortion & distortion, float x, float y, float z, float corr, int index);

        }; // class Glesly::SphereData::Draw

        class Path: public PaCaLib::Path, public Convert3D
        {
            friend class SphereData::Draw;

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
            virtual void Draw(DrawMode mode) override;

            Operations opers;

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
            SYS_DEBUG_MEMBER(DM_GL_SPHERE);
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

static inline std::ostream & operator<<(std::ostream & os, const Glesly::SphereData::Convert3D::Operations::OneOp & op)
{
 op.toStream(os);
 return os;
}

static inline std::ostream & operator<<(std::ostream & os, const Glesly::SphereData::Convert3D::Operations::OneOp::_u & u)
{
 u.toStream(os);
 return os;
}

static inline std::ostream & operator<<(std::ostream & os, const Glesly::SphereData::Convert3D::Operations & ops)
{
 ops.toStream(os);
 return os;
}

#endif /* __SRC_SPHERE_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */

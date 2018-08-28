/**
 *  Copyright 2015 Mike Reed
 */

#ifndef GMatrix_DEFINED
#define GMatrix_DEFINED

#include "GColor.h"
#include "GMath.h"
#include "GPoint.h"
#include "GRect.h"

class GMatrix {
public:
    GMatrix() : fMat{1, 0, 0, 0, 1, 0} {}

    GMatrix(float a, float b, float c, float d, float e, float f) : fMat{a, b, c, d, e, f} {}

    enum {
        SX, KX, TX,
        KY, SY, TY,
    };
    float operator[](int index) const {
        GASSERT(index >= 0 && index < 6);
        return fMat[index];
    }

    bool operator==(const GMatrix& m) {
        for (int i = 0; i < 6; ++i) {
            if (fMat[i] != m.fMat[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const GMatrix& m) { return !(*this == m); }

    static GMatrix MakeTranslate(float tx, float ty) { return GMatrix(1, 0, tx, 0, 1, ty); }

    static GMatrix MakeScale(float sx, float sy) { return GMatrix(sx, 0, 0, 0, sy, 0); }

    static GMatrix MakeScale(float scale) { return GMatrix::MakeScale(scale, scale); }

    // These 4 methods must be implemented by the student.
    /**
     *  Return a matrix to rotate by the specified radians.
     *
     *  Note: since positive-Y goes down, a small angle of rotation will increase Y.
     */
    static GMatrix MakeRotate(float radians);

    /**
     *  Return the concatenation of the two specified matrices, such that the resulting
     *  matrix, when applied to points will have the same effect as first applying the primo matrix
     *  to the points, and then applying the secundo matrix to the resulting points.
     *
     *  Pts' = Secundo * Primo * Pts
     */
    static GMatrix MakeConcat(const GMatrix& secundo, const GMatrix& primo);

    /*
     *  If this matrix is invertible, return true and (if not null) set the inverse parameter.
     *  If this matrix is not invertible, return false and ignore the inverse parameter.
     */
    bool invert(GMatrix* inverse) const;

    /**
     *  Transform the set of points in src, storing the resulting points in dst, by applying this
     *  matrix. It is the caller's responsibility to allocate dst to be at least as large as src.
     *
     *  Note: It is legal for src and dst to point to the same memory (however, they may not
     *  partially overlap). Thus the following is supported.
     *
     *  GPoint pts[] = { ... };
     *  matrix.mapPoints(pts, pts, count);
     */
    void mapPoints(GPoint dst[], const GPoint src[], int count) const;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // These helper methods are implemented in terms of the previous methods.

    GMatrix& set6(float a, float b, float c, float d, float e, float f) {
        return *this = GMatrix{a, b, c, d, e, f};
    }

    GMatrix& setIdentity() {
        return *this = GMatrix();
    }

    GMatrix& setTranslate(float tx, float ty) {
       return *this = GMatrix::MakeTranslate(tx, ty);
    }

    GMatrix& setScale(float sx, float sy) {
       return *this = GMatrix::MakeScale(sx, sy);
    }

    GMatrix& setRotate(float radians) {
        return *this = GMatrix::MakeRotate(radians);
    }

    GMatrix& setConcat(const GMatrix& secundo, const GMatrix& primo) {
        return *this = GMatrix::MakeConcat(secundo, primo);
    }

    GMatrix& preConcat(const GMatrix& primo) {
        return *this = GMatrix::MakeConcat(*this, primo);
    }

    GMatrix& preTranslate(float x, float y) {
        return this->preConcat(GMatrix::MakeTranslate(x, y));
    }

    GMatrix& preScale(float sx, float sy) {
        return this->preConcat(GMatrix::MakeScale(sx, sy));
    }

    GMatrix& preRotate(float radians) {
        return this->preConcat(GMatrix::MakeRotate(radians));
    }

    GMatrix& postConcat(const GMatrix& secundo) {
        return *this = GMatrix::MakeConcat(secundo, *this);
    }

    GMatrix& postTranslate(float x, float y) {
        return this->postConcat(GMatrix::MakeTranslate(x, y));
    }

    GMatrix& postScale(float sx, float sy) {
        return this->postConcat(GMatrix::MakeScale(sx, sy));
    }

    GMatrix& postRotate(float radians) {
        return this->postConcat(GMatrix::MakeRotate(radians));
    }

    GPoint mapXY(float x, float y) const {
        return this->mapPt(GPoint{x, y});
    }

    GPoint mapPt(GPoint pt) const {
        this->mapPoints(&pt, &pt, 1);
        return pt;
    }

private:
    float fMat[6];
};

#endif

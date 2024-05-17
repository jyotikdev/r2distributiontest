//
//  CRDefines.h
//  SquareReader
//
//  Created by Zachary Drayer on 6/13/18.
//

/// CRCardReaderReaderType is suitable for display purposes.
typedef NS_ENUM(NSUInteger, CRCardReaderReaderType) {
    CRCardReaderReaderTypeUnknown = 0,
    CRCardReaderReaderTypeGen2,
    CRCardReaderReaderTypeO1,
    CRCardReaderReaderTypeR4,
    CRCardReaderReaderTypeR6,
    CRCardReaderReaderTypeR12,
    CRCardReaderReaderTypeA10,
    CRCardReaderReaderTypeX2,
    CRCardReaderReaderTypeT2,
};

/// CRCardReaderReaderSubtype is suitable for specific details
/// that differ between readers that otherwise look the same
/// (e.g.: R12a/b vs R12c have the same form factor but different hardware).
typedef NS_ENUM(NSUInteger, CRCardReaderReaderSubtype) {
    CRCardReaderReaderSubtypeUnknown,
    CRCardReaderReaderSubtypeR12ab,
    CRCardReaderReaderSubtypeR12c,
    CRCardReaderReaderSubtypeR12d
};

// Attribution to @robb on GitHub:
// https://gist.github.com/robb/d55b72d62d32deaee5fa

#if __has_feature(objc_generics)

/// An unimplemented class used to trick the compiler, since a cast along the
/// lines of
///
///     (__nonnull __typeof(bla))bla;
///
/// is not possible.
@interface __SQRNonnullConverter<__covariant Type>

- (nonnull Type)asNonNull;

@end

/// This macro allows us to cast a nullable reference to a non-nullable
/// reference that would otherwise trigger a warning if
/// `-Wnullable-to-nonnull-conversion` is enabled.
#define SQRForceNonnull(nullableObject) \
({ \
__auto_type __localForceNonnull = nullableObject; \
NSCAssert(__localForceNonnull, @"Expected '%@' not to be nil.", @#nullableObject); \
__SQRNonnullConverter<__typeof(__localForceNonnull)> *type; \
(__typeof(type.asNonNull))__localForceNonnull; \
})

#else

/// If generics are unavailable, so is `-Wnullable-to-nonnull-conversion`.
#define SQRForceNonnull(nullableObject) \
({ \
NSCAssert(nullableObject, @"Expected '%@' not to be nil.", @#nullableObject); \
nullableObject; \
})

#endif

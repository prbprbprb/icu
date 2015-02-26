/*
 * Copyright (C) 2015, International Business Machines
 * Corporation and others.  All Rights Reserved.
 *
 * file name: pluralaffix.cpp
 */

#include "unicode/utypes.h"
#include "cstring.h"
#include "digitaffix.h"
#include "pluralaffix.h"

U_NAMESPACE_BEGIN

UBool
PluralAffix::setVariant(
        const char *variant, const UnicodeString &value, UErrorCode &status) {
    DigitAffix *current = affixes.getMutable(variant, status);
    if (U_FAILURE(status)) {
        return FALSE;
    }
    current->remove();
    current->append(value);
    return TRUE;
}

void
PluralAffix::remove() {
    affixes.clear();
}

void
PluralAffix::appendUChar(
        const UChar value, int32_t fieldId) {
    PluralMapBase::Variant index = PluralMapBase::NONE;
    for (DigitAffix *current = affixes.nextMutable(index);
            current != NULL; current = affixes.nextMutable(index)) {
        current->appendUChar(value, fieldId);
    }
}

void
PluralAffix::append(
        const UnicodeString &value, int32_t fieldId) {
    PluralMapBase::Variant index = PluralMapBase::NONE;
    for (DigitAffix *current = affixes.nextMutable(index);
            current != NULL; current = affixes.nextMutable(index)) {
        current->append(value, fieldId);
    }
}

UBool
PluralAffix::append(
        const PluralAffix &rhs, int32_t fieldId, UErrorCode &status) {
    if (U_FAILURE(status)) {
        return FALSE;
    }
    PluralMapBase::Variant index = PluralMapBase::NONE;
    while(rhs.affixes.next(index) != NULL) {
        affixes.getMutableWithDefault(index, affixes.getOther(), status);
    }
    index = PluralMapBase::NONE;
    for (DigitAffix *current = affixes.nextMutable(index);
            current != NULL; current = affixes.nextMutable(index)) {
        current->append(rhs.affixes.get(index).toString(), fieldId);
    }
    return TRUE;
}

const DigitAffix &
PluralAffix::getByVariant(const char *variant) const {
    return affixes.get(variant);
}

const DigitAffix &
PluralAffix::getByVariant(const UnicodeString &variant) const {
    return affixes.get(variant);
}

UBool
PluralAffix::hasMultipleVariants() const {
    // This works because OTHER is guaranteed to be the first enum value
    PluralMapBase::Variant index = PluralMapBase::OTHER;
    return (affixes.next(index) != NULL);
}

U_NAMESPACE_END


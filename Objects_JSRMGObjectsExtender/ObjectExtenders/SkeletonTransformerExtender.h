#pragma once


namespace skeletonTransformer {
    // type 144, subtype 1
    constexpr int SKELETON_TRANSFORMER_OBJECT_SUBTYPE = 1;

    struct H3MapItemSkeletonTransformer
    {

    };

    class SkeletonTransformerExtender final
        : public H3ActiveObject<H3MapItemSkeletonTransformer>
    {
    private:
        static SkeletonTransformerExtender* instance;

        SkeletonTransformerExtender();

        BOOL VisitMapItem(
            H3Hero* currentHero,
            H3MapItem* mapItem,
            H3Position pos,
            BOOL isHuman
        ) const noexcept override final;

        //int AI_OnScouting_Value() const noexcept override final;

    public:
        static SkeletonTransformerExtender& Get();
    };
}

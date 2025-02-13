#include "Enemy/C_EWeaponScratch.h"
#include "C_Helpers.h"
#include "Components/SphereComponent.h"

AC_EWeaponScratch::AC_EWeaponScratch()
{
    /***** Sphere Comp *****/
    C_Helpers::CreateComponent<USphereComponent>(this, &Scratch_L, "Scratch_L", RootComponent);

    /***** Equip *****/
    WeaponSocketName = "Scratch_L";
}


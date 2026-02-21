// Fill out your copyright notice in the Description page of Project Settings.

#include "JumpChargeGroupWidget.h"
#include "JumpChargeWidget.h"

void UJumpChargeGroupWidget::RefreshJumpCharges(const int Amount) const
{
    if (Amount == 3)
    {
        Charge0->SetVisible(true);
        Charge1->SetVisible(true);
        Charge2->SetVisible(true);
    }
    else if (Amount == 2)
    {
        Charge0->SetVisible(true);
        Charge1->SetVisible(true);
        Charge2->SetVisible(false);
    }
    else if (Amount == 1)
    {
        Charge0->SetVisible(true);
        Charge1->SetVisible(false);
        Charge2->SetVisible(false);
    }
    else
    {
        Charge0->SetVisible(false);
        Charge1->SetVisible(false);
        Charge2->SetVisible(false);
    }
}

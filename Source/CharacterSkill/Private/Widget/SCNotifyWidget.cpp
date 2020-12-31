// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Widget/SCNotifyWidget.h"
#include "TextBlock.h"
#include "Animation/WidgetAnimation.h"

bool USCNotifyWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	WidgetAnims.Empty();
	UProperty* Prop = GetClass()->PropertyLink;
	/*�������е�UProperty*/
	while (Prop)
	{
		if (Prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* ObjProp = Cast<UObjectProperty>(Prop);
			/*Ѱ�����ǵ�UWidgetAnimation*/
			if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);
				UWidgetAnimation* WidgetAnimation = Cast<UWidgetAnimation>(Obj);
				if (WidgetAnimation && WidgetAnimation->MovieScene)
				{
					/*�Զ�����ΪKey�洢����*/
					FName AnimName = WidgetAnimation->MovieScene->GetFName();
					WidgetAnims.Add(AnimName, WidgetAnimation);
				}
			}
		}
		Prop = Prop->PropertyLinkNext;
	}

	return true;
}

void USCNotifyWidget::UpdateMessage(FText Message)
{
	Text_Message->SetText(Message);
}

#pragma once

#include "AccessibleObject.h"
#include "AccessibleClass.h"

namespace SCFPrivate { void SCFObjectExtensionsInitialize(); };

namespace SCFObjectExtensions
{	
	class OBJECT_EXTENSIONS_API CAccessibleObjectMultiConstructor : public CAccessibleObject
	{
		friend void SCFPrivate::SCFObjectExtensionsInitialize();

	public:
		//Return values CANNOT be deleted if [CProperty::IsReference()] returns true
		CObject* PropertyGet(_IN ENUM eName) _GET throw(...);

	protected:
		CAccessibleObjectMultiConstructor(_IN UINT uiConstructorIndex);
		~CAccessibleObjectMultiConstructor();

	private:
		//Called once on dll load
		static void Register();

	protected:
		static void PropertiesAdd(_OUT CAccessibleClass& rOutClass);

	private:
		UINT m_uiConstructorIndex;

	private:
		struct SProperties
		{
			UINT uiConstructorIndex;

		} static s_Properties;
	};
};
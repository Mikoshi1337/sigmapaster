//struct GameTrace : Il2CppObject {
//	IL2CPP_CLASS("GameTrace");
//
//	static bool Trace(HitTest* test, int layerMask);
//
//	static void TraceAll(uintptr_t a1, uintptr_t a2, int a3)
//	{
//		static uintptr_t procedure = 0;
//		if (!IsAddressValid(procedure))
//		{
//			const auto method = il2cpp::FindMethod(StaticClass(), HASH("TraceAll"), 3);
//			if (IsAddressValid(method))
//			{
//				procedure = ToAddress(method->methodPointer);
//			}
//		}
//
//		if (IsAddressValid(procedure))
//		{
//			return Call<void>(procedure, a1, a2, a3);
//		}
//
//
//		return;
//	}
//};
#pragma once

#if 0


namespace Lithium
{
	using EventCallback = std::function<void(Event&)>;

	class SpriteEditor
	{
		
	public:
		void OnCreate();
		void SetTexture(const Ref<Texture>& texture);
		void OnUpdate();
		void Open()
		{
			_Open = true;
		}
		void Close()
		{
			_Open = false;
		}
		void SetEventCallback(const EventCallback& cb)
		{
			callback = cb;
		}
	private:
		bool _Open = false;
		glm::vec2 cellsize;
		Ref<Texture> _Texture;
		
		bool hasMetadata = false;
		EventCallback callback;
	};
}
#endif
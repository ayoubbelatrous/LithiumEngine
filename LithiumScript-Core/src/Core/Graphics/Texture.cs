using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using Lithium.Mathf;

namespace Lithium.Core
{
    public class Texture
    {
        public int Width;
        public int Height;
        public int Format;
        int id;
        public Texture(int width, int height, int format)
        {
            id = CreateTexture_Internal(width, height, format);
        }
        public void SetData(Vector4[] pixels)
        {


            SetTextureData_Internal(id,pixels,pixels.Length);
        }
        ~Texture()
        {
            DeleteTexture_Internal(id);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static int CreateTexture_Internal(int width, int height, int format);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void DeleteTexture_Internal(int id);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void SetTextureData_Internal(int id,Vector4[] pixels,int size);
    }
}

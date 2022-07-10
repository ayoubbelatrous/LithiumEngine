using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Lithium.Core
{
    public abstract class Script
    {
        public Entity entity;

        protected Script()
        {
            entity = new Entity(0);
        }
        public UInt64 ID
        {
            get
            {
                return entity.ID;
            }
            set
            {

                entity.ID = value;
            }
        }

        public Entity Instantiate(Entity entity)
        {
            UInt64 Id = CopyEntity_Internal(entity.ID);
            Entity DuplicatedEntity = new Entity(Id);

            return DuplicatedEntity;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern UInt64 CopyEntity_Internal(UInt64 ID);
    }

}

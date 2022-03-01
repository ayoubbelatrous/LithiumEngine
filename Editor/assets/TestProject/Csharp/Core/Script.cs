﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lithium.Core
{
    public abstract class Script
    {
        public Entity entity;

        protected Script()
        {
            entity = new Entity(-1);
        }
        public int ID
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
    }

}
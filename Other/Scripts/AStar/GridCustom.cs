﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName = "GridCustom", menuName = "配置/自定义网格")]
public class GridCustom : ScriptableObject {

    public List<CellInfo> m_CellInfo;

    public Color m_StartPointColor;
    public Color m_EndPointColor;
    public Color m_PathColor;

    [System.Serializable]
    public class CellInfo
    {
        public ECellType Type;
        public Color Color;
    }
}

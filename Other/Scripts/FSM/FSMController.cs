﻿using UnityEngine;
using System.Collections.Generic;

[CreateAssetMenu(fileName = "fsm_", menuName = "配置/状态机")]
public class FSMController : ScriptableObject, IFSMMachine
{
    [SerializeField]
    private List<FSMStateAction> m_States;

    [SerializeField]
    private int m_DefaultState;

    [SerializeField]
    private List<FSMParameter> m_Paramaters;

    private int m_CurState;

    private FSMStateAction CurState
    {
        get
        {
            if (m_CurState != -1)
                return GetState(m_CurState) as FSMStateAction;
            return null;
        }
    }

    public List<FSMParameter> Paramaters
    {
        get { return m_Paramaters; }
        set { m_Paramaters = value; }
    }

    public List<FSMStateAction> States
    {
        get { return m_States; }
        set { m_States = value; }
    }

    public int DefaultState
    {
        get { return m_DefaultState; }
        set { m_DefaultState = value; }
    }

    public FSMController() { OnInitialize(); }

    public void OnInitialize()
    {
        States = new List<FSMStateAction>(8);
        Paramaters = new List<FSMParameter>();
        ResetAllTrigger();
        m_CurState = -1;
    }

    public void AddState(IFSMState state)
    {
        if (States.Count <= state.GetId())
        {
            while (States.Count <= state.GetId())
            {
                States.Add(null);
            }
        }
        States[state.GetId()] = state as FSMStateAction;
    }

    public void AddParamater(FSMParameter paramater)
    {
        Paramaters.Add(paramater);
    }

    public void Exit()
    {
        if (CurState != null)
            CurState.OnExit();
    }

    public IFSMState GetState(int id)
    {
        return States[id];
    }

    public void HandleInput(EFSMInputType input, Message msg)
    {
        if (CurState != null)
        {
            int targetId = CurState.HandleInput(input, msg);
            if (targetId != CurState.GetId() && targetId != -1)
            {
                SwitchToState(targetId);
            }
        }
    }

    public void OnUpdate()
    {
        if (CurState == null)
        {
            SwitchToState(DefaultState);
        }

        UpdateTransitions();

        if (CurState != null)
        {
            int targetId = CurState.OnUpdate();
            if (targetId != CurState.GetId())
            {
                SwitchToState(targetId);
            }
        }
    }

    public string GetCurrentStateName()
    {
        if (CurState != null)
            return CurState.StateName;
        return string.Empty;
    }

    Message m_TransitionMsg = new Message();
    public void UpdateTransitions()
    {
        if (CurState == null)
            return;
        m_TransitionMsg["params"] = Paramaters;
        int targetId = CurState.HandleInput(EFSMInputType.FSMCondition, m_TransitionMsg);
        if (targetId != CurState.GetId() && targetId != -1)
            SwitchToState(targetId);

        ResetAllTrigger();
    }

    public void RemoveState(int id)
    {
        States[id] = null;
    }

    public void SetAsDefaultState(IFSMState state)
    {
        DefaultState = state.GetId();
    }

    public void SetDefaultState(int id)
    {
        SetAsDefaultState(GetState(id));
    }

    public void SwitchToState(int id)
    {
        if (CurState != null)
        {
            if (CurState.GetId() == id)
                return;
            CurState.OnExit();
        }
        m_CurState = id;
        if (CurState != null)
            CurState.OnEnter();
    }

    public void SetBool(string paramName, bool value)
    {
        FSMParameter param = GetParameter(paramName);
        if (param != null)
        {
            param.Value.boolValue = value;
        }
        else
        {
            Debug.LogWarning("the param " + paramName + " is not exists");
        }
    }

    public void SetFloat(string paramName, float value)
    {
        FSMParameter param = GetParameter(paramName);
        if (param != null)
        {
            param.Value.floatValue = value;
        }
        else
        {
            Debug.LogWarning("the param " + paramName + " is not exists");
        }
    }

    public void SetTrigger(string paramName)
    {
        FSMParameter param = GetParameter(paramName);
        if (param != null)
        {
            param.Value.triggerValue = true;
            UpdateTransitions();
        }
        else
        {
            Debug.LogWarning("the param " + paramName + " is not exists");
        }
    }

    public void ResetAllTrigger()
    {
        Paramaters.ForEach((p) =>
        {
            if (p.Value.Type == EValueType.Trigger)
                p.Value.triggerValue = false;
        });
    }

    private FSMParameter GetParameter(string paramName)
    {
        return Paramaters.Find((p) =>
        {
            return p.Name == paramName;
        });
    }
}

[System.Serializable]
public class FSMParameter
{
    public string Name;
    public ConditionValue Value;
}

[System.Serializable]
public class FSMCondition
{
    public string Name;
    public ConditionValue Value;
    public EConditionType Type;

    public bool CheckParamater(FSMParameter param)
    {
        bool checkSuccess = false;
        switch (Type)
        {
            case EConditionType.Greater:
                checkSuccess = ConditionValue.Greater(param.Value, Value);
                break;
            case EConditionType.Less:
                checkSuccess = ConditionValue.Less(param.Value, Value);
                break;
            case EConditionType.Whether:
                checkSuccess = ConditionValue.Whether(param.Value, Value);
                break;
            case EConditionType.Trigger:
                checkSuccess = param.Value.triggerValue;
                break;
            case EConditionType.None:
                checkSuccess = true;
                break;
        }
        return checkSuccess;
    }
}

[System.Serializable]
public class ConditionValue
{
    public EValueType Type;

    public string stringValue;

    public int intValue;

    public float floatValue;

    public bool boolValue;

    public bool triggerValue;

    public static bool Greater(ConditionValue v1, ConditionValue v2)
    {
        if (v1.Type == v2.Type)
        {
            if (v1.Type == EValueType.Float)
                return v1.floatValue > v2.floatValue;
            if (v1.Type == EValueType.Int)
                return v1.intValue > v2.intValue;
        }
        else
        {
            throw new System.Exception("v1 type doesn't match to v2");
        }
        return false;
    }

    public static bool Less(ConditionValue v1, ConditionValue v2)
    {
        if (v1.Type == v2.Type)
        {
            if (v1.Type == EValueType.Float)
                return v1.floatValue < v2.floatValue;
            if (v1.Type == EValueType.Int)
                return v1.intValue < v2.intValue;
        }
        else
        {
            throw new System.Exception("v1 type doesn't match to v2");
        }
        return false;
    }

    public static bool Whether(ConditionValue v1, ConditionValue v2)
    {
        if (v1.Type == v2.Type)
        {
            if (v1.Type == EValueType.Bool)
            {
                return v1.boolValue == v2.boolValue;
            }
            else
            {
                throw new System.Exception("v1 and v2 is not boolean values");
            }
        }
        else
        {
            throw new System.Exception("v1 type doesn't match to v2");
        }
    }
}

public enum EConditionType
{
    Greater,
    Less,
    Whether,
    Trigger,
    None,
}

public enum EValueType
{
    Int,
    Float,
    Bool,
    String,
    Trigger,
}

[System.Serializable]
public class FSMTransition
{
    public List<FSMCondition> Conditions;
    public int TargetStateId;
}
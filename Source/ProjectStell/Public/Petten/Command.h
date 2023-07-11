class AActor;

class  Command
{
public:
	Command();
	virtual ~Command();
	virtual void Execute() = 0;
	//virtual void Execute(AActor* actor) = 0;
	virtual void Cancel() = 0; //명령에 대한 실행 취소

};
class MoveCommand : public Command
{
public:
	MoveCommand(AActor* actor, float newAxisValue,bool vectorIsForward) : _actor(actor), _newAxisValue(newAxisValue),_vectorIsForward(vectorIsForward) {}
	// Command을(를) 통해 상속됨
	virtual void Execute() final;
	virtual void Cancel() final;
private:
	AActor* _actor;
	float _newAxisValue;
	bool _vectorIsForward;
};
class AttackCommand : public Command
{
public:
	AttackCommand(AActor* actor, bool isLeftAttack) : _actor(actor), _isLeftAttack(isLeftAttack) {}
	// Command을(를) 통해 상속됨
	virtual void Execute() final;
	virtual void Cancel() final;
private:
	AActor* _actor;
	float _newAxisValue;
	bool _isLeftAttack;
	//여기다 콤보 넣어서 제어할수 있도록 변경하기 
};
class EvasionCommand : public Command
{
public:
	EvasionCommand(AActor* actor) : _actor(actor){}
	// Command을(를) 통해 상속됨
	virtual void Execute() final;
	virtual void Cancel() final;
private:
	AActor* _actor;
};
class EquipmentCommand : public Command
{
public:
	EquipmentCommand(AActor* actor, bool isLeftEquipment) : _actor(actor), _isLeftEquipment(isLeftEquipment){}
	// Command을(를) 통해 상속됨
	virtual void Execute() final;
	virtual void Cancel() final;
private:
	AActor* _actor;
	bool _isLeftEquipment;
};
class ShowGUICommand : public Command
{
public:
	ShowGUICommand(AActor* actor, int32 uiNumber) : _actor(actor), _uiNumber(uiNumber) {}
	// Command을(를) 통해 상속됨
	virtual void Execute() final;
	virtual void Cancel() final;
private:
	AActor* _actor;
	int32 _uiNumber;
};


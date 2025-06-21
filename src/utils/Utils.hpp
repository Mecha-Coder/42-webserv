#define FOR_EACH(Type, Container, It) \
	for (Type::iterator It = (Container).begin(); It != (Container).end(); ++It)
#define FOR_EACH_CONST(Type, Container, It) \
	for (Type::const_iterator It = (Container).begin(); It != (Container).end(); ++It)

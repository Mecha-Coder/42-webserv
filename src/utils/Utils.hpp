/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcheong <rcheong@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:10:16 by rcheong           #+#    #+#             */
/*   Updated: 2025/05/08 21:10:17 by rcheong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define FOR_EACH(Type, Container, It) \
	for (Type::iterator It = (Container).begin(); It != (Container).end(); ++It)
#define FOR_EACH_CONST(Type, Container, It) \
	for (Type::const_iterator It = (Container).begin(); It != (Container).end(); ++It)

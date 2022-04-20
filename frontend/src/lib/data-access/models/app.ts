import { BackendService } from '../service/backend.service';
import { List } from './list'

export interface App {
    lists: List[];
    flaggedCount: number;
    todayCount: number;
    selectedList?: List;
}

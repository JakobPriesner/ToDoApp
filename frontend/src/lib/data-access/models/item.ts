import { BackendService } from '../service/backend.service';

export interface Item {
    id?: number;
    text: string;
    flagged: boolean;
    date?: string;
}
